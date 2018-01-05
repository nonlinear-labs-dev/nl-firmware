package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class TiltColumn extends ParameterColumn {

	private class Tilt extends ModulateableKnob {

		private Tilt(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 192;
		}
	}

	TiltColumn(MapsLayout parent) {
		super(parent);
		addChild(new Tilt(this));
	}
}
