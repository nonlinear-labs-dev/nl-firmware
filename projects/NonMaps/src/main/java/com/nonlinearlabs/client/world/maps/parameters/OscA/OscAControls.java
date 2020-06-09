package com.nonlinearlabs.client.world.maps.parameters.OscA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class OscAControls extends ParameterGroupControls {
	OscAControls(MapsLayout parent) {
		super(parent);
		addChild(new PitchColumn(this));
		addChild(new FluctColumn(this));
		addChild(new PhaseColumn(this));
		addChild(new PMSelfColumn(this));
		addChild(new PMBColumn(this));
		addChild(new PMFBColumn(this));
	}
}
