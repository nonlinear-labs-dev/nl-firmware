package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableKnob extends ModulatableParameter {

	public ModulateableKnob(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulationSourceLabel(this, getParameterNumber()));
		addChild(new KnobLarge(this, getParameterNumber()));
		addChild(new ValueDisplayLarge(this, getParameterNumber()));
	}

}
