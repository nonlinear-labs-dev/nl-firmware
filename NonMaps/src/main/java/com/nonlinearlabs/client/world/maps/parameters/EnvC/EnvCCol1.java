package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class EnvCCol1 extends ParameterColumn {

	EnvCCol1(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 38));
		addChild(new ModulationSourceSlider(this, 49));
		addChild(new SmallKnobParameter(this, 296));
	}
}
