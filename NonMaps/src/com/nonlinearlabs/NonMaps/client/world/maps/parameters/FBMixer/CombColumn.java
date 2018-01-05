package com.nonlinearlabs.NonMaps.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class CombColumn extends ParameterColumn {

	public static class Comb extends ModulatableSlider {

		private Comb(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 156;
		}

	}

	CombColumn(MapsLayout parent) {
		super(parent);
		addChild(new Comb(this));
	}
}
