package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CombFilterCol6 extends ParameterColumn {

	CombFilterCol6(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 129));
		addChild(new ModulatableHorizontalSlider(this, 132));
		addChild(new ModulationSourceSlider(this, 131));

	}
}
