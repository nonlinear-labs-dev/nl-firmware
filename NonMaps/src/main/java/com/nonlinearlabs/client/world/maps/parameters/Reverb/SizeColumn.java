package com.nonlinearlabs.client.world.maps.parameters.Reverb;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class SizeColumn extends ParameterColumn {

	SizeColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 235));
		addChild(new SmallKnobParameter(this, 237));
	}
}
