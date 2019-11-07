package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CutOffColumn extends ParameterColumn {

	CutOffColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 140));
		addChild(new ModulationSourceSlider(this, 143));
		addChild(new ModulationSourceSlider(this, 142));
	}
}
