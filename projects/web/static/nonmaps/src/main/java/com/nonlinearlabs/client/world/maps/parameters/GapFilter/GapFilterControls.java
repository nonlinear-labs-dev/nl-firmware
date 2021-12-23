package com.nonlinearlabs.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class GapFilterControls extends ParameterGroupControls {

	GapFilterControls(MapsLayout parent) {
		super(parent);
		addChild(new StereoColumn(this));
		addChild(new GapColumn(this));
		addChild(new BalanceColumn(this));
		addChild(new MixColumn(this));
	}
}
