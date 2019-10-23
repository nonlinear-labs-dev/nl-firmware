package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class EnvACol3 extends ParameterColumn {

	private class Breakpoint extends ModulateableKnob {

		private Breakpoint(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 4;
		}
	}

	EnvACol3(MapsLayout parent) {
		super(parent);
		addChild(new Breakpoint(this));
	}
}
