package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnobWithoutHeader;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.UnmodulateableNumericDisplay;

class CombFilterCol3 extends ParameterColumn {

	CombFilterCol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 119));
		addChild(new ModulateableSmallKnobParameter(this, 305));
		addChild(new ModulatableHorizontalSlider(this, 121));
	}
}
