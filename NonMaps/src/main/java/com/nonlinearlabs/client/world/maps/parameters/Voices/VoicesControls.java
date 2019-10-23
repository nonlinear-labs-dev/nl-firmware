package com.nonlinearlabs.client.world.maps.parameters.Voices;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class VoicesControls extends ParameterGroupControls {

	VoicesControls(MapsLayout parent) {
		super(parent);
		addChild(new MonoColumn(this));
		addChild(new UnisonColumn(this));
	}
}
