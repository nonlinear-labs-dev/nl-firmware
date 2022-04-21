package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

abstract class ModulationSource extends ModulatableParameter{

	ModulationSource(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}
}
