package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Unison;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class UnisonControls extends ParameterGroupControls {

	UnisonControls(MapsLayout parent) {
		super(parent);
		addChild(new UnisonColumn(this));
	}
}
