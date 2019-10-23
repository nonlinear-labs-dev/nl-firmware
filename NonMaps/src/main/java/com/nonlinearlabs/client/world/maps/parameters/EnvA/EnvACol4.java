package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvACol4 extends ParameterColumn {

	private class Decay2 extends ModulateableKnob {

		private Decay2(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 6;
		}
	}

	EnvACol4(MapsLayout parent) {
		super(parent);
		addChild(new Decay2(this));
	}
}
