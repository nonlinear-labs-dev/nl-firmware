package com.nonlinearlabs.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvBCol3 extends ParameterColumn {

	EnvBCol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 23));
	}
}
