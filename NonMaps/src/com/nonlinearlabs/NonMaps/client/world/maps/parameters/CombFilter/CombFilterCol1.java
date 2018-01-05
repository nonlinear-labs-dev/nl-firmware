package com.nonlinearlabs.NonMaps.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ABSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.InvertedYModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplayLarge;

class CombFilterCol1 extends ParameterColumn {

	private class AB extends InvertedYModulatableParameter {

		private AB(MapsLayout parent) {
			super(parent, "A - B");
			addChild(new ModulationSourceLabel(this));
			addChild(new ABSlider(this));
			addChild(new ValueDisplayLarge(this));
		}

		@Override
		public int getParameterID() {
			return 113;
		}
	}

	CombFilterCol1(MapsLayout parent) {
		super(parent);
		addChild(new AB(this));
	}
}
