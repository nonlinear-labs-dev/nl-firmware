package com.nonlinearlabs.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvBCol6 extends ParameterColumn {

	EnvBCol6(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 29));
		addChild(new ModulationSourceSlider(this, 35));
	}
}
