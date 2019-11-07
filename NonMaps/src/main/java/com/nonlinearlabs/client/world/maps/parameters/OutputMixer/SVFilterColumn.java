package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class SVFilterColumn extends ParameterColumn {

	SVFilterColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 178));
		addChild(new SmallKnobParameter(this, 180));
	}
}
