package com.nonlinearlabs.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class GapColumn extends ParameterColumn {

	GapColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 204));
		addChild(new SmallKnobParameter(this, 206));
	}
}
