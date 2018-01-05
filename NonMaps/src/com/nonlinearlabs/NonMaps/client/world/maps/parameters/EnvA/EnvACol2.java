package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class EnvACol2 extends ParameterColumn {

	private class Decay1 extends ModulateableKnob {

		private Decay1(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 2;
		}
	}

	EnvACol2(MapsLayout parent) {
		super(parent);
		addChild(new Decay1(this));
	}
}
