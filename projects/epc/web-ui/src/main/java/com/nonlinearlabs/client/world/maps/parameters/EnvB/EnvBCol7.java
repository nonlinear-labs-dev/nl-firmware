package com.nonlinearlabs.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvBCol7 extends ParameterColumn {

	EnvBCol7(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 31));
	}
}
