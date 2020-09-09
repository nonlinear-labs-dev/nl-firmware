package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class FlangerCol7 extends ParameterColumn {

	FlangerCol7(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 223));
	}
}
