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
			addChild(new ModulationSourceLabel(this, getParameterID()));
			addChild(new ABSlider(this, getParameterID()));
			addChild(new ValueDisplayLarge(this, getParameterID()));
		}
	}

	CombFilterCol1(MapsLayout parent) {
		super(parent);
		addChild(new AB(this));
	}
}
