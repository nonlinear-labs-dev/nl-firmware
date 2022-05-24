package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ValueDisplayLarge extends ValueDisplay {

	public ValueDisplayLarge(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
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
		return 0;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 5;
	}

}
