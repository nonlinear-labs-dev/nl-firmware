package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvCCol2 extends ParameterColumn {

	EnvCCol2(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 40));
	}
}
