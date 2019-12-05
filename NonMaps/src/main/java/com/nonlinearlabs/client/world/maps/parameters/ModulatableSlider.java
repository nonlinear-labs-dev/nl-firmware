package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulatableSlider extends ModulatableParameter {

	public ModulatableSlider(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulationSourceLabel(this, getParameterNumber()));
		addChild(new SliderVertical(this, getParameterNumber()));
		addChild(new ValueDisplayLarge(this, getParameterNumber()));
	}
}
