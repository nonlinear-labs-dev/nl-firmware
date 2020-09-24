package com.nonlinearlabs.client.world.maps.parameters.Echo;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class TimeColumn extends ParameterColumn {

	TimeColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 225));
		addChild(new ModulateableSmallKnobParameter(this, 227));
	}
}
