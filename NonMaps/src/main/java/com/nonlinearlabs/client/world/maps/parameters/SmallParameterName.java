package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.Name;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class SmallParameterName extends LabelSmall {
	public SmallParameterName(Parameter parent) {
		super(parent);
	}

	@Override
	protected double getBasicHeight() {
		return 20;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	protected String getDisplayText() {
		return "TODO";
	}
}