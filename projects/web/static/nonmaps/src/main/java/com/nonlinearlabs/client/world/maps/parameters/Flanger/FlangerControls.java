package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class FlangerControls extends ParameterGroupControls {

	FlangerControls(MapsLayout parent) {
		super(parent);
		addChild(new RateColumn(this));
		addChild(new EnvAndPhaseColumn(this));
		addChild(new FlangerCol2(this));
		addChild(new FlangerCol3(this));
		addChild(new FlangerCol4(this));
		addChild(new FeedbackColumn(this));
		addChild(new CrossFBAndHicut(this));
		addChild(new FlangerCol7(this));
		addChild(new FlangerCol8(this));
	}
}
