package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class FBMixColumn extends ParameterColumn {

	FBMixColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 108));
		addChild(new ModulatableHorizontalSlider(this, 110));
	}

	public Parameter getSlider() {
		return (Parameter) getChildren().get(0);
	}
}
