package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableKnob extends ModulatableParameter {

	public ModulateableKnob(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulationSourceLabel(this, getParameterID()));
		addChild(new KnobLarge(this, getParameterID()));
		addChild(new ValueDisplayLarge(this, getParameterID()));
	}

}
