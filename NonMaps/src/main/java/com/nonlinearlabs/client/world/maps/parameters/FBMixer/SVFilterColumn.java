package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class SVFilterColumn extends ParameterColumn {

	SVFilterColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 158));
	}
}
