package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class FlangerCol4 extends ParameterColumn {

	FlangerCol4(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 308));
		addChild(new ModulateableSmallKnobParameter(this, 310));
	}
}
