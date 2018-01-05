package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

abstract public class ScaleParameter extends ModulationSourceHighPriority {

	public ScaleParameter(MapsLayout parent) {
		super(parent);
		addChild(new LabelModulationSource(this, getName()));
		addChild(new SliderHorizontal(this));
		addChild(new ValueDisplaySmall(this));
	}

}