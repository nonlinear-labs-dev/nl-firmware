package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class ValueDisplayLarge extends ValueDisplay {

	public ValueDisplayLarge(MapsLayout parent) {
		super(parent);
	}

	@Override
	protected double getFontHeight() {
		return 11;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 25;
	}

	@Override
	protected double getMinHeight() {
		return 15;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 5;
	}

}
