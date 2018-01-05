package com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class SpreadColumn extends ParameterColumn {

	private class Spread extends ModulateableKnob {

		private Spread(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 148;
		}
	}

	SpreadColumn(MapsLayout parent) {
		super(parent);
		addChild(new Spread(this));
	}
}
