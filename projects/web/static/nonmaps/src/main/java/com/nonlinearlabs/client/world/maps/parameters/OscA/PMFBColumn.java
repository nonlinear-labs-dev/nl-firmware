package com.nonlinearlabs.client.world.maps.parameters.OscA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class PMFBColumn extends ParameterColumn {

	PMFBColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 68));
		addChild(new ModulatableHorizontalSlider(this, 70));
		addChild(new SmallKnobParameter(this, 303));
	}
}
