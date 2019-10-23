package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulationSourceSlider extends ModulationSourceHighPriority {

	public ModulationSourceSlider(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelModulationSource(this, getName()));
		addChild(new SliderHorizontal(this));
		addChild(new ValueDisplaySmall(this));
	}
}
