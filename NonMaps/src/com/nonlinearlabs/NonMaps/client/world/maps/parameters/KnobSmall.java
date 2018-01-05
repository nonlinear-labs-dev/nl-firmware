package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class KnobSmall extends Knob {

	public KnobSmall(MapsLayout parent) {
		super(parent);
	}

	@Override
	protected double getCenterPointRadius() {
		return 3;
	}

	@Override
	protected double getInnerRadius() {
		if (shouldDrawThicker())
			return 7;
		return 9;
	}

	@Override
	protected double getOuterRadius() {
		return 15;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 31;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	@Override
	protected double getVerticalOffsetInUnits() {
		return 2;
	}
}
