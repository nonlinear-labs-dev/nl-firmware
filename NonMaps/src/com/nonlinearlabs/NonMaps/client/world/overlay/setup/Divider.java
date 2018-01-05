package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class Divider extends OverlayControl {

	public Divider(Control parent) {
		super(parent);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, Millimeter.toPixels(0.25));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().fill(ctx, new Gray(69));
	}

}
