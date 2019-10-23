package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class ModulationSourceHighPriority extends ModulationSource {

	public ModulationSourceHighPriority(MapsLayout parent) {
		super(parent);
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 4;
	}
}
