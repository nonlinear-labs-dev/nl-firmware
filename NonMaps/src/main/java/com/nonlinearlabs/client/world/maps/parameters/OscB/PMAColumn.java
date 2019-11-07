package com.nonlinearlabs.client.world.maps.parameters.OscB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class PMAColumn extends ParameterColumn {

	PMAColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 94));
		addChild(new ModulationSourceSlider(this, 96));
		addChild(new SmallKnobParameter(this, 97));
	}
}
