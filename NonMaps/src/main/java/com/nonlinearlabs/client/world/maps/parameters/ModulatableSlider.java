package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class ModulatableSlider extends ModulatableParameter {

	public ModulatableSlider(MapsLayout parent) {
		super(parent);
		addChild(new ModulationSourceLabel(this));
		addChild(new SliderVertical(this));
		addChild(new ValueDisplayLarge(this));
	}
}
