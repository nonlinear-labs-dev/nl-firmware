package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class EnvBCol7 extends ParameterColumn {

	private class Gain extends ModulatableSlider {

		private Gain(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 31;
		}
	}

	EnvBCol7(MapsLayout parent) {
		super(parent);
		addChild(new Gain(this));
	}
}
