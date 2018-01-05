package com.nonlinearlabs.NonMaps.client.world.maps.parameters;


import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public abstract class ModulationSourceHighPriority extends ModulationSource {

	public ModulationSourceHighPriority(MapsLayout parent) {
		super(parent);
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 4;
	}

}
