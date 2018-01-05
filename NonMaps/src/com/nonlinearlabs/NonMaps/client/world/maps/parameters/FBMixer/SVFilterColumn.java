package com.nonlinearlabs.NonMaps.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class SVFilterColumn extends ParameterColumn {

	private class SVF extends ModulatableSlider {

		private SVF(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 158;
		}
	}

	SVFilterColumn(MapsLayout parent) {
		super(parent);
		addChild(new SVF(this));
	}
}
