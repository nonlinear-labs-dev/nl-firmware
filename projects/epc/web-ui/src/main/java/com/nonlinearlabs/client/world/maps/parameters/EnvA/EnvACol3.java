package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvACol3 extends ParameterColumn {

	EnvACol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 4));
	}
}
