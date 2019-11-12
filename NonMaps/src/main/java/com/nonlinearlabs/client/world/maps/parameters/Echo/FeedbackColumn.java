package com.nonlinearlabs.client.world.maps.parameters.Echo;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FeedbackColumn extends ParameterColumn {

	FeedbackColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 229));
		addChild(new SmallKnobParameter(this, 231));
	}
}
