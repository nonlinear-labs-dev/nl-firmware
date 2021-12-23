package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class ModulationSourceHighPriority extends ModulationSource {

	public ModulationSourceHighPriority(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 4;
	}
}
