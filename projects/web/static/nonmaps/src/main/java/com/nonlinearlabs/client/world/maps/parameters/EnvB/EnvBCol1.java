package com.nonlinearlabs.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class EnvBCol1 extends ParameterColumn {

	EnvBCol1(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 19));
		addChild(new ModulationSourceSlider(this, 34));
		addChild(new SmallKnobParameter(this, 295));
	}
}
