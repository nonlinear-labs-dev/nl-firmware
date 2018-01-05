package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Reverb;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;

class ReverbControls extends ParameterGroupControls {

	ReverbControls(MapsLayout parent) {
		super(parent);
		addChild(new SizeColumn(this));
		addChild(new FilterColumn(this));
		addChild(new MixColumn(this));
	}
}
