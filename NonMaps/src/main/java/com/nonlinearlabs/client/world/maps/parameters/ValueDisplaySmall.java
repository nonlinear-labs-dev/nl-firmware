package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ValueDisplaySmall extends ValueDisplay {

	public ValueDisplaySmall(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	protected double getFontHeight() {
		return 10;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 19;
	}

	@Override
	protected double getMinHeight() {
		return 7.5;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 5;
	}
}
