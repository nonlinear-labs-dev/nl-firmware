package com.nonlinearlabs.client.world.maps.parameters.Reverb;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class SizeColumn extends ParameterColumn {

	SizeColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 235));
		addChild(new SmallKnobParameter(this, 237));
	}
}
