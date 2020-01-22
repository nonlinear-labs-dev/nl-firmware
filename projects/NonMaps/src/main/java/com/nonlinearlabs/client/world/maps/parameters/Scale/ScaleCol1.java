package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.world.maps.MapsLayout;

class ScaleCol1 extends ScaleCol {

	ScaleCol1(MapsLayout parent) {
		super(parent);

		addChild(new ScaleParameter(this, 312));
		addChild(new ScaleParameter(this, 316));
		addChild(new ScaleParameter(this, 320));
	}
}
