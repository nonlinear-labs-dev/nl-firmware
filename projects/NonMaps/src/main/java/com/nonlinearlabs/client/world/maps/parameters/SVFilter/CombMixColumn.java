package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CombMixColumn extends ParameterColumn {

	CombMixColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 138));

	}
}
