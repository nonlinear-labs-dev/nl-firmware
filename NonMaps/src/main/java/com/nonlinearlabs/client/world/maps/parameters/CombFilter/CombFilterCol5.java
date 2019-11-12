package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CombFilterCol5 extends ParameterColumn {

	CombFilterCol5(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 127));
	}
}
