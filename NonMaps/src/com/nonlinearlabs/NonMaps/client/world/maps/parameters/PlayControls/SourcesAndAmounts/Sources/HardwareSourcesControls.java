package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class HardwareSourcesControls extends ParameterGroupControls {

	HardwareSourcesControls(MapsLayout parent) {
		super(parent);

		addChild(new HardwareSourcesCol1(this));
	}
}
