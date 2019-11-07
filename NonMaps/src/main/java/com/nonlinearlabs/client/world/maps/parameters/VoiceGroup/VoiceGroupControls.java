package com.nonlinearlabs.client.world.maps.parameters.VoiceGroup;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class VoicesGroupControls extends ParameterGroupControls {

	VoicesGroupControls(MapsLayout parent) {
		super(parent);
		addChild(new VoiceGroupColumn(this));
	}
}
