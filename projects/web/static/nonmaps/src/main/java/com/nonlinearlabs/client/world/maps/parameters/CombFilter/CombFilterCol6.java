package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.UnmodulateableNumericDisplay;

class CombFilterCol6 extends ParameterColumn {

	CombFilterCol6(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 129));
		addChild(new ModulatableHorizontalSlider(this, 132));
		addChild(new ModulatableHorizontalSlider(this, 131));
	}
}
