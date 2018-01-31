package com.nonlinearlabs.NonMaps.client.world.overlay;

import java.util.ArrayList;
import java.util.LinkedList;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.Viewport;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;
import com.nonlinearlabs.NonMaps.client.world.overlay.menu.GlobalMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.setup.Setup;
import com.nonlinearlabs.NonMaps.client.world.overlay.undo.UndoTreeWindow;
import com.nonlinearlabs.NonMaps.client.world.pointer.Down;
import com.nonlinearlabs.NonMaps.client.world.pointer.Gesture;

public class Overlay extends OverlayLayout {

	private Belt belt = null;
	private GlobalButtons buttons = null;
	private UndoRedoButtons undoRedo = null;
	private boolean layoutRequested = false;
	private Setup setup;
	private boolean initialLayout = true;
	private GlobalMenu globalMenu;
	private UndoTreeWindow undo;

	public Overlay(Viewport parent) {
		super(parent);
		addChild(belt = new Belt(this, parent.getNonMaps()));
		addChild(buttons = new GlobalButtons(this, belt));
		addChild(undoRedo = new UndoRedoButtons(this, belt));
		addChild(setup = new Setup(this));
		addChild(undo = new UndoTreeWindow(this));

		// has to be the last one to add as it refers to previously added
		// members
		addChild(globalMenu = new GlobalMenu(this));
	}

	
	public void refreshGlobalMenu() {
		globalMenu.refresh();
	}
	
	@Override
	public Viewport getParent() {
		return (Viewport) super.getParent();
	}

	@Override
	public NonMaps getNonMaps() {
		return getParent().getNonMaps();
	}

	@Override
	public boolean isRectVisible(Rect r) {
		return true;
	}

	@Override
	public void requestLayout() {
		layoutRequested = true;
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (layoutRequested) {
			layoutRequested = false;
			doLayout(0, 0, getRelativePosition().getWidth(), getRelativePosition().getHeight());
		}

		buttons.drawInactiveButton(ctx, invalidationMask);
		drawBackground(ctx);
		buttons.drawActiveButton(ctx, invalidationMask);

		super.draw(ctx, invalidationMask);
	}

	private void drawBackground(Context2d ctx) {
		Rect r = belt.getPixRect();

		double corner = Millimeter.toPixels(1);
		double activeButtonLeft = buttons.getLeftOfActiveButton();
		double activeButtonRight = buttons.getRightOfActiveButton();
		double activeButtonTop = buttons.getPixRect().getTop();

		ctx.beginPath();
		ctx.moveTo(r.getLeft(), r.getBottom());
		ctx.lineTo(r.getLeft(), r.getTop() + corner);
		ctx.arcTo(r.getLeft(), r.getTop(), r.getLeft() + corner, r.getTop(), corner);

		if (belt.isHidden()) {
			ctx.lineTo(r.getRight() - corner, r.getTop());
			ctx.arcTo(r.getRight(), r.getTop(), r.getRight(), r.getTop() + corner, corner);

		} else {

			ctx.lineTo(activeButtonLeft - corner, r.getTop());
			ctx.arcTo(activeButtonLeft, r.getTop(), activeButtonLeft, r.getTop() - corner, corner);
			ctx.lineTo(activeButtonLeft, activeButtonTop + corner);
			ctx.arcTo(activeButtonLeft, activeButtonTop, activeButtonLeft + corner, activeButtonTop, corner);
			ctx.lineTo(activeButtonRight - corner, activeButtonTop);
			ctx.arcTo(activeButtonRight, activeButtonTop, activeButtonRight, activeButtonTop + corner, corner);
			ctx.lineTo(activeButtonRight, r.getTop() - corner);
			ctx.arcTo(activeButtonRight, r.getTop(), activeButtonRight + corner, r.getTop(), corner);

			if (activeButtonRight != r.getRight()) {
				ctx.lineTo(r.getRight() - corner, r.getTop());
				ctx.arcTo(r.getRight(), r.getTop(), r.getRight(), r.getTop() + corner, corner);
			}
		}

		ctx.lineTo(r.getRight(), r.getBottom());
		ctx.lineTo(r.getLeft(), r.getBottom());
		ctx.closePath();
		ctx.setLineWidth(5);
		ctx.setStrokeStyle(new Gray(0).toString());
		ctx.stroke();

		ctx.setFillStyle(getColorBackground().toString());
		ctx.fill();

		ctx.setLineWidth(2);
		ctx.setStrokeStyle(new Gray(78).toString());
		ctx.stroke();
	}

	public Gray getColorBackground() {
		return new Gray(51);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		getRelativePosition().moveTo(0, 0);
		super.doLayout(x, y, w, h);

		double beltHeight = SVGImage.calcSVGDimensionToPixels(110);
		belt.doLayout(0, h - beltHeight, w, beltHeight);
		beltHeight = belt.getRelativePosition().getHeight();

		double undoWidth = SVGImage.calcSVGDimensionToPixels(52);
		double undoHeight = SVGImage.calcSVGDimensionToPixels(32);
		double undoRedoMargin = SVGImage.calcSVGDimensionToPixels(5);
		undoRedo.doLayout(undoRedoMargin, h - beltHeight - undoHeight - undoRedoMargin, undoWidth * 2 + undoRedoMargin, undoHeight
				+ undoRedoMargin);

		double singleButtonWidth = getButtonDimension() * 2;
		double buttonsWidth = 3 * singleButtonWidth;
		double buttonsHeight = undoHeight;
		buttons.doLayout(w - buttonsWidth, h - beltHeight - buttonsHeight, buttonsWidth, buttonsHeight);

		layoutFloatingWindows(undoRedoMargin);
		layoutContextMenus(w, h);

		globalMenu.doLayout(w, h);

		calcPixRect(new Position(0, 0), 1.0);
		initialLayout = false;
	}

	private void layoutFloatingWindows(double initialWindowOffset) {
		int i = 0;

		for (OverlayControl c : getChildren()) {
			if (c instanceof FloatingWindow) {
				i++;
				FloatingWindow wnd = (FloatingWindow) c;
				if (initialLayout)
					wnd.doLayout(i * initialWindowOffset, i * initialWindowOffset);
				else
					wnd.doLayout(wnd.getRelativePosition().getLeft(), wnd.getRelativePosition().getTop());
			}
		}
	}

	protected void layoutContextMenus(double w, double h) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof ContextMenu) {
				ContextMenu m = (ContextMenu) c;
				double menuWidth = m.getDesiredWidth();
				double menuHeight = m.getDesiredHeight();
				double menuX = m.getRelativePosition().getLeft();
				double menuY = m.getRelativePosition().getTop();

				if (menuX + menuWidth > w)
					menuX = w - menuWidth;

				if (menuY + menuHeight > h)
					menuY = h - menuHeight;

				m.doLayout(menuX, menuY, menuWidth, menuHeight);
			}
		}
	}

	@Override
	public RGB getColorFont() {
		return NonMaps.theMaps.getNonLinearWorld().getColorFont();
	}

	@Override
	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	@Override
	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

	public DragProxy createDragProxy(Control origin, Position point) {
		removeDragProxies();
		return addDragProxy(origin, point);
	}

	public DragProxy addDragProxy(Control origin, Position point) {
		return addChild(new DragProxy(this, origin, point));
	}

	boolean removeDragProxies() {
		for (OverlayControl c : getChildren()) {
			if (c instanceof DragProxy) {
				removeChild(c);
				removeDragProxies();
				return true;
			}
		}
		return false;
	}

	public void removeDragProxy(DragProxy c) {
		removeChild(c);
	}

	public void cancelDragging() {
		removeDragProxies();
	}

	public boolean isCurrentlyDragging(Control client) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof DragProxy) {
				DragProxy p = (DragProxy) c;
				if (p.getOrigin() == client) {
					return true;
				}
			}
		}

		return false;
	}

	public boolean isCurrentlyDraggingATypeOf(String className) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof DragProxy) {
				DragProxy p = (DragProxy) c;
				if (p.getOrigin().getClass().getName().equals(className))
					return true;
			}
		}

		return false;
	}

	public com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt getBelt() {
		return belt;
	}

	public void update(Node settingsNode, Node editBufferNode, Node presetManagerNode, Node deviceInfo, Node undoNode) {
		setup.update(settingsNode, deviceInfo);
		undo.update(undoNode);
		belt.update(settingsNode, editBufferNode, presetManagerNode);
		undoRedo.update();
	}

	public ContextMenu setContextMenu(Position pt, ContextMenu ctx) {
		removeExistingContextMenus();
		addChild(ctx);
		ctx.getRelativePosition().moveTo(pt.getX() - getPixRect().getLeft(), pt.getY() - getPixRect().getTop());
		requestLayout();
		return ctx;
	}

	private boolean removeExistingContextMenus() {
		boolean any = false;
		for (OverlayControl c : getChildren()) {
			if (c instanceof ContextMenu) {
				ContextMenu m = (ContextMenu) c;
				m.fadeOut();
				any = true;
			}
		}
		return any;
	}

	private void collapseGlobalMenu() {
		for (OverlayControl c : getChildren()) {
			if (c instanceof GlobalMenu) {
				if (((GlobalMenu) c).getState() == GlobalMenu.State.Expanded) {
					((GlobalMenu) c).toggle();
				}
			}
		}
	}

	@Override
	public Control handleGesture(Gesture g) {
		if (g instanceof Down) {
			if (!hitsContextMenu(((Down) g).getPosition())) {
				removeExistingContextMenus();
			}
			if (!hitsGlobalMenu(((Down) g).getPosition())) {
				collapseGlobalMenu();
			}
		}

		return super.handleGesture(g);
	}

	private boolean hitsContextMenu(Position position) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof ContextMenu) {
				if (c.getPixRect().contains(position))
					return true;
			}
		}
		return false;
	}

	private boolean hitsGlobalMenu(Position position) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof GlobalMenu) {
				if (c.getPixRect().contains(position))
					return true;
			}
		}
		return false;
	}

	public Setup getSetup() {
		return setup;
	}

	public FloatingWindow getPresetSearch() {
		return null;
	}

	public UndoTreeWindow getUndoTree() {
		return undo;
	}

	public void pushToTop(FloatingWindow w) {
		ArrayList<OverlayControl> c = getChildren();
		if (c.get(c.size() - 1) != w) {

			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}

		for (OverlayControl ctrl : getChildren()) {
			if (ctrl instanceof ContextMenu) {
				c.remove(ctrl);
				c.add(ctrl);
				break;
			}
		}

	}

	public void moveDragProxies(double xDiff, double yDiff) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof DragProxy) {
				DragProxy d = (DragProxy) c;
				d.moveProxy(xDiff, yDiff);
			}
		}

	}

	public LinkedList<DragProxy> getDragProxies() {
		LinkedList<DragProxy> proxies = new LinkedList<DragProxy>();
		for (OverlayControl c : getChildren()) {
			if (c instanceof DragProxy) {
				DragProxy d = (DragProxy) c;
				proxies.add(d);
			}
		}
		return proxies;
	}

	public DragProxy getDragProxyFor(Control origin) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof DragProxy) {
				DragProxy d = (DragProxy) c;
				if (d.getOrigin() == origin)
					return d;
			}
		}
		return null;
	}
}
