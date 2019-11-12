package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulationSourceSlider extends ModulationSourceHighPriority {

	public ModulationSourceSlider(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelModulationSource(this, getParameterID()));
		addChild(new SliderHorizontal(this, getParameterID()));
		addChild(new ValueDisplaySmall(this, getParameterID()));
	}
}
