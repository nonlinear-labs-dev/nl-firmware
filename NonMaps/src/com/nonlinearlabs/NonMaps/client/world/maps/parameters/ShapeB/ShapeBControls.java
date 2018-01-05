package com.nonlinearlabs.NonMaps.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class ShapeBControls extends ParameterGroupControls {

	ShapeBControls(MapsLayout parent) {
		super(parent);
		addChild(new DriveColumn(this));
		addChild(new DriveExtColumn(this));
		addChild(new MixColumn(this));
		addChild(new FBMixColumn(this));
		addChild(new RingModColumn(this));
	}

}
