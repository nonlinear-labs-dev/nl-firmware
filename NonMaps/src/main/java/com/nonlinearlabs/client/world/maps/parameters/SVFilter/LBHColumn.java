package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class LBHColumn extends ParameterColumn {

	LBHColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 150));
		addChild(new SmallKnobParameter(this, 152));
	}
}
