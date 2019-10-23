package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EffectsColumn extends ParameterColumn {

	private class FX extends ModulatableSlider {

		private FX(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 160;
		}
	}

	EffectsColumn(MapsLayout parent) {
		super(parent);
		addChild(new FX(this));
	}
}
