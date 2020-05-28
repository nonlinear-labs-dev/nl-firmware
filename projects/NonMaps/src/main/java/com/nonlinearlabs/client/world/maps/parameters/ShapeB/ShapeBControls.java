package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class ShapeBControls extends ParameterGroupControls {

	ShapeBControls(MapsLayout parent) {
		super(parent);
		addChild(new DriveColumn(this));
		addChild(new FoldAsymMixColumn(this));
		addChild(new FBMixAndRingModColumns(this));
	}

}
