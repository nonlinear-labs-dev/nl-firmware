package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.world.maps.MapsLayout;

class ScaleCol2 extends ScaleCol {

	ScaleCol2(MapsLayout parent) {
		super(parent);
		addChild(new ScaleParameter(this, 313));
		addChild(new ScaleParameter(this, 317));
		addChild(new ScaleParameter(this, 321));
	}
}
