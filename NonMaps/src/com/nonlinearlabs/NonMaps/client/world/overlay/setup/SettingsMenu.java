package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public abstract class SettingsMenu extends SettingsControl {

	Label label;

	public SettingsMenu(Control parent) {
		super(parent);

		addChild(label = new CenteredSetupLabel(this, "") {

			@Override
			public String getDrawText(Context2d ctx) {
				return getChoices()[getChoice()];
			}
		});
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		double buttonDim = getButtonDimension();
		double maxWidth = Millimeter.toPixels(35);
		if (w > maxWidth)
			w = maxWidth;

		label.doLayout(0, 0, w - (h * 0.5), buttonDim);
		super.doLayout(x, y, w, h);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getReducedBackgroundRect().fillAndStroke(ctx, new Gray(69), Millimeter.toPixels(0.25), getLineColor());
		super.draw(ctx, invalidationMask);
		drawPopupMenuButton(ctx);
	}

	private Gray getLineColor() {
		return new Gray(100);
	}

	private Rect getReducedBackgroundRect() {
		Rect r = getPixRect().copy();
		double margin = r.getHeight() * 0.25;
		r.applyPadding(0, margin, 0, margin);
		return r;
	}

	@Override
	public RGB getColorFont() {
		return new Gray(150);
	}

	private void drawPopupMenuButton(Context2d ctx) {
		Rect r = getReducedBackgroundRect();
		double buttonDim = r.getHeight();
		ctx.setStrokeStyle(getLineColor().toString());
		ctx.setLineWidth(Millimeter.toPixels(0.25));
		ctx.beginPath();
		ctx.moveTo(r.getRight() - buttonDim, r.getTop());
		ctx.lineTo(r.getRight() - buttonDim, r.getBottom());
		ctx.stroke();

		ctx.setLineWidth(Millimeter.toPixels(0.5));
		ctx.beginPath();
		ctx.setStrokeStyle(new Gray(90).toString());
		double lineWidth = buttonDim / 2;
		double margin = lineWidth / 2;
		double heightPerStroke = r.getHeight() / 4;
		double h = heightPerStroke;

		for (int i = 0; i < 3; i++) {
			ctx.moveTo(r.getRight() - buttonDim + margin, r.getTop() + h);
			ctx.lineTo(r.getRight() - margin, r.getTop() + h);
			ctx.stroke();
			h += heightPerStroke;
		}
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
		ContextMenu c = new ContextMenu(o);
		String[] choices = getChoices();

		for (int i = 0; i < choices.length; i++) {
			final int index = i;
			c.addChild(new ContextMenuItem(c, choices[i]) {
				@Override
				public Control click(Position eventPoint) {
					chose(index, true);
					return super.click(eventPoint);
				}
			});
		}

		o.setContextMenu(eventPoint, c);
		return c;
	}

	protected void fromSettingsString(String str) {
		int idx = settingsStringToIndex(str);
		if (idx >= 0)
			chose(idx, false);
	}

	protected int settingsStringToIndex(String str) {
		if (str != null && !str.isEmpty()) {
			String choices[] = getChoices();
			int len = choices.length;

			for (int i = 0; i < len; i++) {
				if (choices[i].replace(" ", "-").toLowerCase().equals(str)) {
					return i;
				}
			}
		}
		return -1;
	}

	public String getCurrentChoiceString() {
		return getChoices()[getChoice()];
	}

	protected abstract String[] getChoices();

	protected abstract int getChoice();

	protected abstract void chose(int c, boolean fire);

}
