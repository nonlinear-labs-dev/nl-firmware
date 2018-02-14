package com.nonlinearlabs.NonMaps.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class MenuHeader extends OverlayControl {

	public MenuHeader(GlobalMenu menu) {
		super(menu);
	}

	@Override
	public GlobalMenu getParent() {
		return (GlobalMenu) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		Rect r = getPixRect();
		double buttonDim = r.getHeight();
		ctx.setStrokeStyle(getColorFont().toString());
		ctx.setLineWidth(Millimeter.toPixels(0.5));

		double lineWidth = buttonDim / 2;
		double margin = lineWidth / 2;
		double yMargin = lineWidth / 4;
		double heightPerStroke = (r.getHeight() - 2 * yMargin) / 4;
		double h = heightPerStroke + yMargin;

		ctx.beginPath();

		for (int i = 0; i < 3; i++) {
			ctx.moveTo(r.getRight() - buttonDim + margin, r.getTop() + h);
			ctx.lineTo(r.getRight() - margin, r.getTop() + h);
			h += heightPerStroke;
		}

		ctx.stroke();
	}

	@Override
	public Control click(Position eventPoint) {
		getParent().toggle();
		return this;
	}

	@Override
	public Control doubleClick() {
		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

}
