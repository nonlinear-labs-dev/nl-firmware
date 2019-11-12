package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FilterColumn extends ParameterColumn {

	FilterColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 194));
		addChild(new SmallKnobParameter(this, 196));
	}
}
