package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SetupButton extends CenteredSetupLabel {

	public SetupButton(OverlayLayout parent, String label) {
		super(parent, label);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		Rect r = getPixRect();
		r.drawValueEditSliderBackgound(ctx, false, getColorFont());
		super.draw(ctx, invalidationMask);
	}

}