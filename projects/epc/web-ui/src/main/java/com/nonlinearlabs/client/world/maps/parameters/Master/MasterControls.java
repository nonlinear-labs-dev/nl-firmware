package com.nonlinearlabs.client.world.maps.parameters.Master;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class MasterControls extends ParameterGroupControls {

	MasterControls(MapsLayout parent) {
		super(parent);
		addChild(new MasterCol1(this));
	}
}
