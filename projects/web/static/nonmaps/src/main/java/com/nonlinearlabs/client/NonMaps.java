package com.nonlinearlabs.client;

import java.util.ArrayList;

import com.google.gwt.canvas.client.Canvas;
import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.google.gwt.core.client.Scheduler.ScheduledCommand;
import com.google.gwt.dom.client.DivElement;
import com.google.gwt.dom.client.Document;
import com.google.gwt.dom.client.Element;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.ui.RootPanel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.DisplayScaling;
import com.nonlinearlabs.client.localStorage.WebStorage;
import com.nonlinearlabs.client.useCases.LocalSettings;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Mouseing;
import com.nonlinearlabs.client.world.NonLinearWorld;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.CachingMapsControl;
import com.nonlinearlabs.client.world.maps.NonRect;
import com.nonlinearlabs.client.world.overlay.BankInfoDialog;
import com.nonlinearlabs.client.world.overlay.CompareDialog;
import com.nonlinearlabs.client.world.overlay.PresetInfoDialog;

public class NonMaps extends Mouseing implements EntryPoint {
	public interface ScreenResizeListener {
		void onScreenResize(int width, int height);
	}

	private ArrayList<ScreenResizeListener> screenResizeListeners = new ArrayList<ScreenResizeListener>();

	private Canvas canvas;
	private Context2d context;

	private NonLinearWorld nonlinearWorld;
	private final ServerProxy server = new ServerProxy(this);

	private static Dimension pixPerCM = new Dimension();
	private static Dimension pixPerViewport = new Dimension();
	private static double pixelFactor = 1.0f;

	private int viewportWidth = 0;
	private int viewportHeight = 0;

	private boolean mouseCaptured = false;

	private static double mmToPixel = 1;
	private static double screenRatio = 1.0;

	private static final int refreshRate = 25;

	private boolean zoomToFitParamed = true;
	private ScheduledCommand refreshBitmapCaches;

	public NonLinearWorld getNonLinearWorld() {
		return nonlinearWorld;
	}

	public static NonMaps theMaps = null;

	@Override
	public void onModuleLoad() {
		exportGlobalMethods();

		LocalSettings.get().setStorage(new WebStorage());
		SetupModel.get().localSettings.displayScaling.onChange(s -> setScaling(s));

		Tracer.registerCrashHandler();

		GWT.setUncaughtExceptionHandler(new GWT.UncaughtExceptionHandler() {
			public void onUncaughtException(Throwable e) {
				Tracer.log(e.getMessage());
				Tracer.sendHistoryToServer();
			}
		});

		assert (theMaps == null);

		theMaps = this;

		RootPanel p = RootPanel.get("cm");

		pixPerCM.setWidth(p.getOffsetWidth() / 100.0);
		pixPerCM.setHeight(p.getOffsetWidth() / 100.0);
		p.setVisible(false);

		p = RootPanel.get("vh");

		pixPerViewport.setWidth(p.getOffsetWidth());
		pixPerViewport.setHeight(p.getOffsetHeight());
		p.setVisible(false);

		mmToPixel = pixelFactor * getDevicePixelRatio() * pixPerCM.getWidth() / 10;

		createWorld();

		try {
			createCanvases();
		} catch (Exception e) {
			e.printStackTrace();
		}

		initHandlers(canvas);
		setupTimer();

		server.startPolling();
	}

	private boolean setScaling(DisplayScaling s) {
		switch (s) {
			case percent_100:
				setPixelFactor(1.0);
				break;
			case percent_125:
				setPixelFactor(1.25);
				break;
			case percent_150:
				setPixelFactor(1.5);
				break;
			case percent_50:
				setPixelFactor(0.5);
				break;
			case percent_75:
				setPixelFactor(0.75);
				break;
		}

		return true;
	}

	public static NonMaps get() {
		return theMaps;
	}

	public static double mmToPixels(double mm) {
		return mm * mmToPixel;
	}

	public static double getPixelFactor() {
		return pixelFactor;
	}

	public Canvas getCanvas() {
		return canvas;
	}

	public void setPixelFactor(double f) {
		if (pixelFactor != f) {
			pixelFactor = f;

			if (nonlinearWorld != null)
				nonlinearWorld.requestLayout();
		}
	}

	private void setupTimer() {
		final RepeatingCommand cmd = new RepeatingCommand() {

			@Override
			public boolean execute() {
				doUpdate();
				return true;
			}
		};

		final Timer timer = new Timer() {
			@Override
			public void run() {
				cmd.execute();
			}
		};

		timer.scheduleRepeating(refreshRate);
	}

	private void createCanvases() throws Exception {
		canvas = Canvas.createIfSupported();
		canvas.getElement().setId("nonmaps-canvas");
		RootPanel.get("canvasholder").add(canvas);
		context = canvas.getContext2d();
	}

	private void createWorld() {
		nonlinearWorld = new NonLinearWorld(this);
		nonlinearWorld.init();

		DivElement presetSearchResults = (DivElement) Document.get().getElementById("preset-search-results");
		observe(presetSearchResults);
	}

	private void doUpdate() {
		double devicePixelRatio = getDevicePixelRatio();
		int width = (int) Math.ceil(canvas.getCanvasElement().getClientWidth() * devicePixelRatio);
		int height = (int) Math.ceil(canvas.getCanvasElement().getClientHeight() * devicePixelRatio);
		mmToPixel = pixelFactor * devicePixelRatio * pixPerCM.getWidth() / 10;

		boolean initial = viewportHeight == 0;

		if (viewportWidth != width || viewportHeight != height) {
			double relCenterX = 0.5;
			double relCenterY = 0.5;
			double pixPerNonX = 1;
			double pixPerNonY = 1;

			if (!initial) {
				NonRect paramedPos = nonlinearWorld.getParameterEditor().getNonPosition();
				NonRect vpPos = nonlinearWorld.getViewport().getNonPosition();

				relCenterX = (vpPos.getCenterPoint().getX() - paramedPos.getLeft()) / paramedPos.getWidth();
				relCenterY = (vpPos.getCenterPoint().getY() - paramedPos.getTop()) / paramedPos.getHeight();
				pixPerNonX = nonlinearWorld.getViewport().getPixRect().getWidth() / vpPos.getWidth();
				pixPerNonY = nonlinearWorld.getViewport().getPixRect().getHeight() / vpPos.getHeight();
			}

			viewportWidth = width;
			viewportHeight = height;
			screenRatio = (double) width / height;

			canvas.setCoordinateSpaceWidth(width);
			canvas.setCoordinateSpaceHeight(height);

			nonlinearWorld.setScreenSize(width, height);

			if (!initial) {
				NonRect paramedPos = nonlinearWorld.getParameterEditor().getNonPosition();
				NonRect vpPos = nonlinearWorld.getViewport().getNonPosition();

				double x = (relCenterX * paramedPos.getWidth()) + paramedPos.getLeft();
				double y = (relCenterY * paramedPos.getHeight()) + paramedPos.getTop();

				double minPixPerNon = Math.min(pixPerNonX, pixPerNonY);
				double newWidth = width / minPixPerNon;
				double newHeight = height / minPixPerNon;
				vpPos.setDimension(newWidth, newHeight);

				vpPos.moveTo(x - newWidth / 2, y - newHeight / 2);

				for (ScreenResizeListener r : screenResizeListeners)
					r.onScreenResize(width, height);

				updatePresetSearchHeight();

			} else {
				zoomToFitParamed = true;
			}

			nonlinearWorld.requestLayout();
		}

		int invalidationMask = nonlinearWorld.getAndResetInvalid();

		if (invalidationMask != 0) {
			nonlinearWorld.draw(context, invalidationMask);

			if (refreshBitmapCaches == null) {
				refreshBitmapCaches = new ScheduledCommand() {

					@Override
					public void execute() {
						refreshBitmapCaches = null;
						CachingMapsControl.doSomeRedraws(10);
					}
				};

				Scheduler.get().scheduleDeferred(refreshBitmapCaches);
			}
		}

		if (zoomToFitParamed) {
			zoomToFitParamed = false;
			nonlinearWorld.zoomTo(nonlinearWorld.getParameterEditor());
		}
	}

	public ServerProxy getServerProxy() {
		return server;
	}

	@Override
	public void invalidate(int flags) {
		NonLinearWorld world = getNonLinearWorld();
		if (world != null)
			world.invalidate(flags);
	}

	public boolean handleKeyPress(KeyDownEvent event) {
		return nonlinearWorld.handleKeyPress(event);
	}

	@Override
	public void captureFocus() {
		canvas.setFocus(true);
	}

	public void captureMouse() {
		DOM.setCapture(NonMaps.theMaps.getCanvas().getCanvasElement());
		mouseCaptured = true;
	}

	public void releaseMouse() {
		DOM.releaseCapture(NonMaps.theMaps.getCanvas().getCanvasElement());
		mouseCaptured = false;
	}

	public boolean isMouseCaptured() {
		return mouseCaptured;
	}

	public double getScreenRatio() {
		return screenRatio;
	}

	public static native double getDevicePixelRatio() /*-{
														return $wnd.devicePixelRatio;
														}-*/;

	public void registerScreenResizeListener(ScreenResizeListener listener) {
		screenResizeListeners.add(listener);
	}

	public void unregisterScreenResizeListener(ScreenResizeListener listener) {
		screenResizeListeners.remove(listener);
	}

	public void updatePresetSearchHeight() {
		double devicePixelRatio = getDevicePixelRatio();
		DivElement presetSearch = (DivElement) Document.get().getElementById("preset-search");
		double beltTop = getNonLinearWorld().getViewport().getOverlay().getUndoRedoButtons().getPixRect().getTop()
				/ devicePixelRatio;
		double margin = Millimeter.toPixels(5) / devicePixelRatio;
		beltTop -= margin;

		String oldVal = presetSearch.getAttribute("belt-position");
		String newVal = String.valueOf(beltTop);
		if (newVal != oldVal) {
			presetSearch.setAttribute("belt-position", newVal + "px");
			presetSearch.getStyle().setProperty("maxHeight", newVal + "px");
		}
	}

	private native double observe(Element element)
	/*-{
		var self = this;
		var observer = new MutationObserver(function(mutations) {
			mutations.forEach(function(mutation) {
	  			if (mutation.type === "attributes") {
					self.@com.nonlinearlabs.client.NonMaps::updatePresetSearchHeight()();
	  			}
			});
		});	
	
		observer.observe(element, {
			attributes: true //configure it to listen to attribute changes
		});
	}-*/;

	private static native double getElementTop(Element element)
	/*-{
	   var rectObject = element.getBoundingClientRect();
	   return rectObject.top;
	}-*/;

	private static native double getElementBottom(Element element)
	/*-{
	   var rectObject = element.getBoundingClientRect();
	   return rectObject.bottom;
	}-*/;

	@Override
	public void startPresetDrag(Position p, String[] presets) {
		getNonLinearWorld().startPresetDrag(p, presets);
	}

	@Override
	public void cancelPresetDrag() {
		getNonLinearWorld().cancelPresetDrag();
	}

	public static void scrollToPreset(String preset) {
		get().getNonLinearWorld().getPresetManager().findPreset(preset).scrollToCenter();
	}

	public static boolean isPresetInfoVisible() {
		return PresetInfoDialog.isShown();
	}

	public static void showPresetInfo() {
		PresetInfoDialog.toggle();
	}

	public static boolean isBankInfoVisible() {
		return BankInfoDialog.isShown();
	}

	public static void showBankInfo() {
		BankInfoDialog.toggle();
	}

	public static void redrawNonWorld() {
		NonMaps.get().nonlinearWorld.invalidate(Control.INVALIDATION_FLAG_UI_CHANGED);
	}

	public static void comparePresetToEditbuffer(String presetId) {
		CompareDialog.open(get().nonlinearWorld.getPresetManager().findPreset(presetId));
	}

	public static void comparePresets(String lhs, String rhs) {
		var pm = get().nonlinearWorld.getPresetManager();
		CompareDialog.open(pm.findPreset(lhs), pm.findPreset(rhs));
	}

	public static native void togglePresetSearch() /*-{
		$wnd.togglePresetSearch();
	}-*/;

	public static native boolean isPresetSearchVisible() /*-{
		return $wnd.isPresetSearchVisible();
	}-*/;

	private static native void exportGlobalMethods() /*-{
		$wnd.scrollToNonMapsPreset = @com.nonlinearlabs.client.NonMaps::scrollToPreset(Ljava/lang/String;);
		$wnd.isPresetInfoVisible = @com.nonlinearlabs.client.NonMaps::isPresetInfoVisible();
		$wnd.showPresetInfo = @com.nonlinearlabs.client.NonMaps::showPresetInfo();
		$wnd.isBankInfoVisible = @com.nonlinearlabs.client.NonMaps::isBankInfoVisible();
		$wnd.showBankInfo = @com.nonlinearlabs.client.NonMaps::showBankInfo();
		$wnd.nextZIndex = @com.nonlinearlabs.client.NonMaps::nextZIndex();
		$wnd.redrawNonWorld = @com.nonlinearlabs.client.NonMaps::redrawNonWorld();
		$wnd.comparePresetToEditbuffer = @com.nonlinearlabs.client.NonMaps::comparePresetToEditbuffer(Ljava/lang/String;);
		$wnd.comparePresets = @com.nonlinearlabs.client.NonMaps::comparePresets(Ljava/lang/String;Ljava/lang/String;);
	}-*/;

	public static native int nextZIndex() /*-{
		if(!$wnd.maxDialogZIndex)
			$wnd.maxDialogZIndex = 1001;
			
		$wnd.maxDialogZIndex++;
		return $wnd.maxDialogZIndex;
	}-*/;
}
