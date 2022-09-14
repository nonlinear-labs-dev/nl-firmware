package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CombFilterCol4 extends ParameterColumn {

	CombFilterCol4(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 123));
		addChild(new ModulatableHorizontalSlider(this, 126));
		addChild(new ModulateableSmallKnobParameter(this, 125));
	}
}
