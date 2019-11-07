package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class MixColumn extends ParameterColumn {

	MixColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 106));
	}
}
