package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulatableSlider extends ModulatableParameter {

	public ModulatableSlider(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulationSourceLabel(this, getParameterID()));
		addChild(new SliderVertical(this, getParameterID()));
		addChild(new ValueDisplayLarge(this, getParameterID()));
	}
}
