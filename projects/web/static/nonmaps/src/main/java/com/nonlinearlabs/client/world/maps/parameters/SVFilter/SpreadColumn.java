package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class SpreadColumn extends ParameterColumn {

	SpreadColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 148));
	}
}
