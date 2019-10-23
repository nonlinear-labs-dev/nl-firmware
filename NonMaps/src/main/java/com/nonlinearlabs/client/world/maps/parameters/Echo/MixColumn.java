package com.nonlinearlabs.client.world.maps.parameters.Echo;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class MixColumn extends ParameterColumn {

	private class Mix extends ModulatableSlider {

		private Mix(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 233;
		}
	}

	MixColumn(MapsLayout parent) {
		super(parent);
		addChild(new Mix(this));
	}
}
