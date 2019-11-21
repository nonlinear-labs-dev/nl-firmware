package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FlangerCol3 extends ParameterColumn {

	FlangerCol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 216));
		addChild(new SmallKnobParameter(this, 218));
	}
}
