package com.nonlinearlabs.NonMaps.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.BankInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.ParameterInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.SearchQueryDialog;

public class GlobalMenu extends OverlayLayout {

	public enum State {
		Collapsed, Expanded
	};

	private State state = State.Collapsed;
	private MenuHeader header;
	private Label headerText;

	public GlobalMenu(Overlay overlay) {
		super(overlay);

		addChild(headerText = new GlobalMenuHeaderLabel(this));
		addChild(header = new MenuHeader(this));
		addChild(new ToggleViewMenuEntry(this, overlay.getSetup()));
		addChild(new MenuEntry(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return (SearchQueryDialog.isShown() ? "\uE0A4 " : "\uE0A3 ") + "Preset Search";
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				SearchQueryDialog.toggle();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				getParent().toggle();
				return this;
			}
		});

		addChild(new ToggleViewMenuEntry(this, overlay.getUndoTree()));
		
		addChild(new MenuEntry(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return (PresetInfoDialog.isShown() ? "\uE0A4 " : "\uE0A3 ") + "Preset Info";
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				PresetInfoDialog.toggle();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				getParent().toggle();
				return this;
			}
		});

		addChild(new MenuEntry(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return (BankInfoDialog.isShown() ? "\uE0A4 " : "\uE0A3 ") + "Bank Info";
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				BankInfoDialog.toggle();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				getParent().toggle();
				return this;
			}
		});

		addChild(new MenuEntry(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return (ParameterInfoDialog.isShown() ? "\uE0A4 " : "\uE0A3 ") + "Parameter Info";
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				ParameterInfoDialog.toggle();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				getParent().toggle();
				return this;
			}
		});

		addChild(new MenuEntry(this) {
			@Override
			public String getDrawText(Context2d ctx) {
				return (getNonMaps().getNonLinearWorld().getPresetManager().getPresetsVisible() ? "\uE0A4 " : "\uE0A3 ") + "Presets";
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				getNonMaps().getNonLinearWorld().getPresetManager().toggleHiddenBanks();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				getParent().toggle();
				return this;
			}
		});
		
		addChild(new MenuEntry(this) {
			
			@Override
			public String getDrawText(Context2d ctx) {
				return (getNonMaps().getNonLinearWorld().getParameterEditor().isEditorVisible() ? "\uE0A4 " : "\uE0A3 ") + "Parameter Editor";
			}
			
			@Override
			public Control mouseDown(Position eventPoint) {
				getNonMaps().getNonLinearWorld().getParameterEditor().toggleVisibility();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				getParent().toggle();
				return this;
			}
		});

		addChild(new MenuEntry(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return "\uE0A3 Open Help";
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				getParent().toggle();
				Window.open("/NonMaps/war/online-help/index.html", "", "");
				return this;
			}

		});

	}

	@Override
	public Overlay getParent() {
		return (Overlay) super.getParent();
	}

	public void doLayout(double w, double h) {
		double buttonDim = getButtonDimension();
		double top = Millimeter.toPixels(2);

		if (state == State.Collapsed) {
			double myWidth = buttonDim;
			double myHeight = buttonDim;
			super.doLayout(w - myWidth, top, myWidth, myHeight);
			header.doLayout(0, 0, buttonDim, buttonDim);
			headerText.doLayout(0, 0, 0, 0);

			for (OverlayControl c : getChildren()) {
				if (c instanceof MenuEntry) {
					c.doLayout(0, 0, 0, 0);
				}
			}
		} else {
			final int IGNORED_CHILDREN = 1;
			
			double myWidth = Millimeter.toPixels(45);
			int childCount = getChildren().size() - IGNORED_CHILDREN;
			double myHeight = childCount * buttonDim;
			super.doLayout(w - myWidth, top, myWidth, myHeight);
			header.doLayout(0, 0, myWidth, buttonDim);
			headerText.doLayout(0, 0, 70, buttonDim);
			double y = buttonDim;

			for (OverlayControl c : getChildren()) {
				if (c instanceof MenuEntry) {
					c.doLayout(0, y, myWidth, buttonDim);
					y += buttonDim;
				}
			}
		}
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		double lineWidth = Millimeter.toPixels(1);

		createBackgroundAreaPath(ctx);
		ctx.setStrokeStyle(RGB.black().toString());
		ctx.setLineWidth(lineWidth);
		ctx.stroke();

		ctx.setFillStyle(RGB.floatingWindowHeaderBackground().toString());
		ctx.fill();

		super.draw(ctx, invalidationMask);

		createBackgroundAreaPath(ctx);
		ctx.setStrokeStyle(RGB.floatingWindowHeaderBorder().toString());
		ctx.setLineWidth(lineWidth / 4);
		ctx.stroke();
	}

	private void createBackgroundAreaPath(Context2d ctx) {
		Rect r = getPixRect();
		double corner = Millimeter.toPixels(1);
		ctx.beginPath();
		ctx.moveTo(r.getRight(), r.getTop());
		ctx.lineTo(r.getLeft() + corner, r.getTop());
		ctx.arcTo(r.getLeft(), r.getTop(), r.getLeft(), r.getTop() + corner, corner);
		ctx.lineTo(r.getLeft(), r.getBottom() - corner);
		ctx.arcTo(r.getLeft(), r.getBottom(), r.getLeft() + corner, r.getBottom(), corner);
		ctx.lineTo(r.getRight(), r.getBottom());
		ctx.lineTo(r.getRight(), r.getTop());
	}

	public void toggle() {
		if (state == State.Collapsed)
			state = State.Expanded;
		else
			state = State.Collapsed;
		requestLayout();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public Control doubleClick() {
		return this;
	}

	@Override
	public Control click(Position eventPoint) {
		return this;
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		return this;
	}

	public State getState() {
		return state;
	}

}
