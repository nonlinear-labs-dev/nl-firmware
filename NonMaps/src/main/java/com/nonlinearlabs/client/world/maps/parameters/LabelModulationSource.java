package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.Name;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class LabelModulationSource extends SmallParameterName {

	public LabelModulationSource(Parameter parent) {
		super(parent);
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return getParent().getLevelOfDetailForFullVisibility();
	}
}
