package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscB;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class OscBControls extends ParameterGroupControls {

	OscBControls(MapsLayout parent) {
		super(parent);
		addChild(new PitchColumn(this));
		addChild(new FluctColumn(this));
		addChild(new PMSelfColumn(this));
		addChild(new PMAColumn(this));
		addChild(new PMFBColumn(this));
	}
}
