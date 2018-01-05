package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public abstract class ModulateableKnob extends ModulatableParameter {

	protected ModulateableKnob(MapsLayout parent) {
		super(parent);
		addChild(new ModulationSourceLabel(this));
		addChild(new KnobLarge(this));
		addChild(new ValueDisplayLarge(this));
	}

}
