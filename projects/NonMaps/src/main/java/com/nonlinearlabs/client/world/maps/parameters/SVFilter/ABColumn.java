package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ABSlider;
import com.nonlinearlabs.client.world.maps.parameters.InvertedYModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class ABColumn extends ParameterColumn {

	private class AB extends InvertedYModulatableParameter {

		private AB(MapsLayout parent) {
			super(parent, "A - B", 136);
			addChild(new ModulationSourceLabel(this, getParameterNumber()));
			addChild(new ABSlider(this, getParameterNumber()));
			addChild(new ValueDisplayLarge(this, getParameterNumber()));
		}

	}

	ABColumn(MapsLayout parent) {
		super(parent);
		addChild(new AB(this));
	}
}
