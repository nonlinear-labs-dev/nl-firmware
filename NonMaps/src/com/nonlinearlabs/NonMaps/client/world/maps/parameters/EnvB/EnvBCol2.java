package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class EnvBCol2 extends ParameterColumn {

	private class Decay1 extends ModulateableKnob {

		private Decay1(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 21;
		}
	}

	EnvBCol2(MapsLayout parent) {
		super(parent);
		addChild(new Decay1(this));
	}
}
