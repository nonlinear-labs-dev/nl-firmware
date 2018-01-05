package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class EnvBCol4 extends ParameterColumn {

	private class Decay2 extends ModulateableKnob {

		private Decay2(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 25;
		}
	}

	EnvBCol4(MapsLayout parent) {
		super(parent);
		addChild(new Decay2(this));
	}
}
