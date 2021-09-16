package com.nonlinearlabs.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class StereoColumn extends ParameterColumn {

	StereoColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 201));
		addChild(new SmallKnobParameter(this, 203));
	}
}
