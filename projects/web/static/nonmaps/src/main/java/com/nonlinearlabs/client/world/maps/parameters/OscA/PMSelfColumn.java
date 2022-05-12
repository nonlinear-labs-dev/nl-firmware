package com.nonlinearlabs.client.world.maps.parameters.OscA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class PMSelfColumn extends ParameterColumn {

	PMSelfColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 60));
		addChild(new ModulatableHorizontalSlider(this, 62));
		addChild(new SmallKnobParameter(this, 63));
	}
}
