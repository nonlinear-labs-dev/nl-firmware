package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.world.maps.Label;
import com.nonlinearlabs.client.world.maps.MapsLayout;

class Name extends Label {

	Name(MapsLayout parent, String text) {
		super(parent, text);
	}

	@Override
	protected TextAlign getTextAlignment() {
		return TextAlign.LEFT;
	}

	@Override
	public double getLeftMargin() {
		return 2.5;
	}

	@Override
	public double getRightMargin() {
		return 2.5;
	}

	@Override
	protected double getBasicWidth() {
		return super.getBasicWidth() + 10; // + 10 to match the grid
	}

}
