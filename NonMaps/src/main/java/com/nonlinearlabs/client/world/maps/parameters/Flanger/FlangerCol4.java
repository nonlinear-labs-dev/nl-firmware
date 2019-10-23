package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class FlangerCol4 extends ParameterColumn {

	private class APMod extends ModulateableKnob {

		private APMod(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 308;
		}
	}

	FlangerCol4(MapsLayout parent) {
		super(parent);
		addChild(new APMod(this));
	}
}
