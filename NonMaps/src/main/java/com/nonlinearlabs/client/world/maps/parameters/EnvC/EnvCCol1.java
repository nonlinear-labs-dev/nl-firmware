package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class EnvCCol1 extends ParameterColumn {

	EnvCCol1(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 38));
		addChild(new ModulationSourceSlider(this, 49));
		addChild(new SmallKnobParameter(this, 296));
	}
}
