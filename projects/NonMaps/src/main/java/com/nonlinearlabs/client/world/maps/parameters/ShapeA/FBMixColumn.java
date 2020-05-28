package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class FBMixColumn extends ParameterColumn {

	FBMixColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 78));
		addChild(new ModulationSourceSlider(this, 80));
	}

	public Parameter getSlider() {
		return (Parameter)getChildren().get(0);
	}
}
