package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class NumericalControlSmall extends NumericalControlLarge {

	public NumericalControlSmall(MapsLayout parent) {
		super(parent);
	}

	@Override
	protected double getFontHeight() {
		return 11;
	}

	@Override
	protected double getInsetX() {
		return 5;
	}

	@Override
	protected double getInsetY() {
		return 6;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 27;
	}
}
