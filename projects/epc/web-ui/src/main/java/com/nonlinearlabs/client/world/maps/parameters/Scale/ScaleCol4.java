package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.world.maps.MapsLayout;

class ScaleCol4 extends ScaleCol {

	ScaleCol4(MapsLayout parent) {
		super(parent);
		addChild(new ScaleParameter(this, 315));
		addChild(new ScaleParameter(this, 319));
		addChild(new ScaleParameter(this, 323));
	}
}
