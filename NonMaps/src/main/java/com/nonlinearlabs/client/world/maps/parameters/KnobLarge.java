package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class KnobLarge extends Knob {

	public KnobLarge(MapsLayout parent, int parameterId) {
		super(parent, parameterId);
	}

	@Override
	protected double getVerticalOffsetInUnits() {
		return 0;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 33;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	@Override
	protected double getCenterPointRadius() {
		if (shouldDrawThicker())
			return 4.5;
		return 3.5;
	}

	@Override
	protected double getInnerRadius() {
		if (shouldDrawThicker())
			return getOuterRadius() - 10;
		return getOuterRadius() - 8;
	}

	@Override
	protected double getOuterRadius() {
		return getBasicHeight() / 1.5;
	}
}
