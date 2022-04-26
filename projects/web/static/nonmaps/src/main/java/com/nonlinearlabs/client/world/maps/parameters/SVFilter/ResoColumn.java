package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class ResoColumn extends ParameterColumn {

	ResoColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 144));
		addChild(new ModulatableHorizontalSlider(this, 147));
		addChild(new ModulationSourceSlider(this, 146));
	}
}
