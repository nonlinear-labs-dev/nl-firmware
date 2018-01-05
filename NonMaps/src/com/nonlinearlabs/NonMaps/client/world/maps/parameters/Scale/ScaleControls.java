package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class ScaleControls extends ParameterGroupControls {

	ScaleControls(MapsLayout parent) {
		super(parent);
		addChild(new ScaleCol1(this));
		addChild(new ScaleCol2(this));
		addChild(new ScaleCol3(this));
		addChild(new ScaleCol4(this));
	}
}
