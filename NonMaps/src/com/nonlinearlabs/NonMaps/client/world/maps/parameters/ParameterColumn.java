package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public abstract class ParameterColumn extends LayoutResizingVertical {

	public ParameterColumn(MapsLayout parent) {
		super(parent);
	}

	@Override
	public double getPadding() {
		return 2;
	}

	@Override
	public double getYMargin() {
		return 2;
	}

	@Override
	public double getBottomMargin() {
		return super.getBottomMargin() + 1;
	}

}