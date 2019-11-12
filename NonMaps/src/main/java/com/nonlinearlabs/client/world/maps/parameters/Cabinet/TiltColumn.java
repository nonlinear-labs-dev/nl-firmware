package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class TiltColumn extends ParameterColumn {

	TiltColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 192));
	}
}
