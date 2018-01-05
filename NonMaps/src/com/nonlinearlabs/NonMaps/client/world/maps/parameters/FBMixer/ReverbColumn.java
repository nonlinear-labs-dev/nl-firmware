package com.nonlinearlabs.NonMaps.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class ReverbColumn extends ParameterColumn {

	private class Reverb extends ModulatableSlider {

		private Reverb(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 162;
		}
	}

	ReverbColumn(MapsLayout parent) {
		super(parent);
		addChild(new Reverb(this));
	}
}
