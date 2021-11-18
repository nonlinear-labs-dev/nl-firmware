package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.LayoutResizingHorizontal;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class TripleParameterColumn extends LayoutResizingHorizontal {

	public TripleParameterColumn(MapsLayout parent) {
		super(parent);
	}

	@Override
	public double getPadding() {
		return 2;
	}

	@Override
	public double getYMargin() {
		return 0;
	}
}