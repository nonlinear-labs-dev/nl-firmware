package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class ShapeAControls extends ParameterGroupControls {

	ShapeAControls(MapsLayout parent) {
		super(parent);
		addChild(new DriveColumn(this));
		addChild(new FoldAsymMixColumn(this));
		FBMixColumn fb = addChild(new FBMixColumn(this));
		addChild(new RingModColumn(this, fb.getSlider()));
	}

}
