package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class ShapeAControls extends ParameterGroupControls {

	ShapeAControls(MapsLayout parent) {
		super(parent);
		addChild(new DriveColumn(this));
		FoldColumn fold = addChild(new FoldColumn(this));
		AsymColumn asym = addChild(new AsymColumn(this));
		MixColumn mix = addChild(new MixColumn(this));
		addChild(new FBMixColumn(this));
		addChild(new RingModColumn(this));

		fold.setReferenceControl(mix);
		asym.setReferenceControl(mix);
	}

}
