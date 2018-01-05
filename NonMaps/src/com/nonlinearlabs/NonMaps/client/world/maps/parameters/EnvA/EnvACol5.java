package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class EnvACol5 extends ParameterColumn {

	private class Sustain extends ModulateableKnob {

		private Sustain(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 8;
		}
	}

	EnvACol5(MapsLayout parent) {
		super(parent);
		addChild(new Sustain(this));
	}
}
