package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class EnvBCol3 extends ParameterColumn {

	private class Breakpoint extends ModulateableKnob {

		private Breakpoint(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 23;
		}
	}

	EnvBCol3(MapsLayout parent) {
		super(parent);
		addChild(new Breakpoint(this));
	}
}
