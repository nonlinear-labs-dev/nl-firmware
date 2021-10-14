package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class LevelColumn extends ParameterColumn {

	LevelColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 197));
	}
}
