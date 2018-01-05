package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Master;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class MasterControls extends ParameterGroupControls {

	MasterControls(MapsLayout parent) {
		super(parent);
		addChild(new MasterCol1(this));
	}
}
