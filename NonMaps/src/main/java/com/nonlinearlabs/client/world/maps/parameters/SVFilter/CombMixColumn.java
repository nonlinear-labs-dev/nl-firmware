package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CombMixColumn extends ParameterColumn {

	private class CombMix extends ModulatableSlider {

		private CombMix(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 138;
		}
	}

	CombMixColumn(MapsLayout parent) {
		super(parent);
		addChild(new CombMix(this));

	}
}
