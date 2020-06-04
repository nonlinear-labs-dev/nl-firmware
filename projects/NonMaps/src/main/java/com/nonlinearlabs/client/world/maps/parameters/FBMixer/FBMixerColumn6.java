package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FBMixerColumn6 extends ParameterColumn {

	FBMixerColumn6(MapsLayout parent) {
		super(parent);
		addChild(new SmallKnobParameter(this, 166));
		addChild(new SmallKnobParameter(this, 167));
	}
}
