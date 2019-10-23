package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvCCol2 extends ParameterColumn {

	private class Decay extends ModulateableKnob {

		private Decay(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 40;
		}
	}

	EnvCCol2(MapsLayout parent) {
		super(parent);
		addChild(new Decay(this));
	}
}
