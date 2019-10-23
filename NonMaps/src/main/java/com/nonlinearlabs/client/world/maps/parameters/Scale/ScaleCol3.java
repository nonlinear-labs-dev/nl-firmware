package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.world.maps.MapsLayout;

class ScaleCol3 extends ScaleCol {

	ScaleCol3(MapsLayout parent) {
		super(parent);
		addChild(new ScaleParameter(this, 314));
		addChild(new ScaleParameter(this, 318));
		addChild(new ScaleParameter(this, 322));
	}
}
