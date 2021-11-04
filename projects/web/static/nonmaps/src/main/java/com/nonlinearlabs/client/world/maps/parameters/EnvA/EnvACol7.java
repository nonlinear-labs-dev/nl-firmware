package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvACol7 extends ParameterColumn {

	EnvACol7(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 12));
	}
}
