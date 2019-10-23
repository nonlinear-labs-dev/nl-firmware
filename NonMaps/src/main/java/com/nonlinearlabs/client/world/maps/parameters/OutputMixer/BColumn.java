package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class BColumn extends ParameterColumn {

	BColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 172));
		addChild(new SmallKnobParameter(this, 174));
	}
}
