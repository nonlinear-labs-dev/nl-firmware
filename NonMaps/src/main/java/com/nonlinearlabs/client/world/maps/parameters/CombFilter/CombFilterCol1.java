package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ABSlider;
import com.nonlinearlabs.client.world.maps.parameters.InvertedYModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class CombFilterCol1 extends ParameterColumn {

	private class AB extends InvertedYModulatableParameter {

		private AB(MapsLayout parent) {
			super(parent, "A - B", 113);
			addChild(new ModulationSourceLabel(this));
			addChild(new ABSlider(this));
			addChild(new ValueDisplayLarge(this));
		}
	}

	CombFilterCol1(MapsLayout parent) {
		super(parent);
		addChild(new AB(this));
	}
}
