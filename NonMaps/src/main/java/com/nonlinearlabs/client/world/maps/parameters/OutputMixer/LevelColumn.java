package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class LevelColumn extends ParameterColumn {

	LevelColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 185));
		addChild(new SmallKnobParameter(this, 187));
	}
}
