package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class FBMixColumn extends ParameterColumn {

	private class EnvC extends ModulationSourceHighPriority {

		private EnvC(MapsLayout parent) {
			super(parent, 110);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	FBMixColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 108));
		addChild(new EnvC(this));
	}
}
