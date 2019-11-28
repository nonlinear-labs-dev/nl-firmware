package com.nonlinearlabs.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvBCol5 extends ParameterColumn {

	EnvBCol5(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 27));
	}
}