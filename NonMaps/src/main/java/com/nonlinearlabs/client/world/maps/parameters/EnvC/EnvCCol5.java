package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvCCol5 extends ParameterColumn {

	private class Sustain extends ModulateableKnob {

		private Sustain(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 297;
		}
	}

	EnvCCol5(MapsLayout parent) {
		super(parent);
		addChild(new Sustain(this));
	}
}
