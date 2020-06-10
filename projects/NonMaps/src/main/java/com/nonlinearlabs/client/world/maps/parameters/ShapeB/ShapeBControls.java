package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class ShapeBControls extends ParameterGroupControls {

	ShapeBControls(MapsLayout parent) {
		super(parent);
		addChild(new DriveColumn(this));
		addChild(new FoldColumn(this));
		addChild(new AsymColumn(this));
		addChild(new MixColumn(this));
		addChild(new FBMixColumn(this));
		addChild(new RingModColumn(this));
	}

}
