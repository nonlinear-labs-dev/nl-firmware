package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.RingModKnob;

class RingModColumn extends ParameterColumn {

	RingModColumn(MapsLayout parent) {
		super(parent);
		addChild(new RingModKnob(this, 111));
	}

}
