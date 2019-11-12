package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvACol6 extends ParameterColumn {

	EnvACol6(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 10));
		addChild(new ModulationSourceSlider(this, 16));
	}
}
