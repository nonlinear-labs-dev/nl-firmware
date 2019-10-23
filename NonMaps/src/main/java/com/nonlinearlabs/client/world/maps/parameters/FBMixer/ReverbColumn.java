package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class ReverbColumn extends ParameterColumn {

	ReverbColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 162));
	}
}
