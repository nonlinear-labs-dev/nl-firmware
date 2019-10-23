package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvACol7 extends ParameterColumn {

	private class Gain extends ModulatableSlider {

		private Gain(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 12;
		}
	}

	EnvACol7(MapsLayout parent) {
		super(parent);
		addChild(new Gain(this));
	}
}
