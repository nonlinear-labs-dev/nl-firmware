package com.nonlinearlabs.NonMaps.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class MixColumn extends ParameterColumn {

	private class Mix extends ModulatableSlider {

		private Mix(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 76;
		}
	}

	MixColumn(MapsLayout parent) {
		super(parent);
		addChild(new Mix(this));
	}
}
