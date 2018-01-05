package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class FlangerCol7 extends ParameterColumn {

	private class Mix extends ModulatableSlider {

		private Mix(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 223;
		}
	}

	FlangerCol7(MapsLayout parent) {
		super(parent);
		addChild(new Mix(this));
	}
}
