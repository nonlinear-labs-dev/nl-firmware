package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class LabelLarge extends LabelSmall {

	public LabelLarge(Parameter parent) {
		super(parent);
	}

	public LabelLarge(MapsLayout parent, String text) {
		super(parent, text);
	}

	@Override
	protected double getFontHeight() {
		return 12;
	}

	@Override
	protected double getBasicWidth() {
		return 70;
	}

	@Override
	protected double getBasicHeight() {
		return 20;
	}

	@Override
	protected double getMinHeight() {
		return 10;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return getParent().getLevelOfDetailForFullVisibility() + 1;
	}
}
