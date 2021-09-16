package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.world.maps.Label;
import com.nonlinearlabs.client.world.maps.MapsLayout;

class Number extends Label {

	public Number(MapsLayout parent, String text) {
		super(parent, text);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		setNonSize(32.5 + getWidthMargin(), getNonPosition().getHeight());
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
		return 0;
	}
}
