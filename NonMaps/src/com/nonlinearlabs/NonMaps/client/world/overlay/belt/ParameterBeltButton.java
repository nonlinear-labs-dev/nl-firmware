package com.nonlinearlabs.NonMaps.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Tracer;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class ParameterBeltButton extends BeltButton {

	public ParameterBeltButton(OverlayLayout parent, Belt belt) {
		super(parent, belt, "Param_Tab_Enabled.svg", "Param_Tab_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		if (!isActive()) {
			belt.setParameterView(true);
		} else {
			belt.toggle();
		}
		return this;
	}

	@Override
	public boolean isActive() {
		return belt.isParameterView();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		Tracer.log("draw parameter belt button");
		super.draw(ctx, invalidationMask);
	}

}
