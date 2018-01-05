package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class FlangerCol5 extends ParameterColumn {

	private class APTune extends ModulateableKnob {

		private APTune(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 310;
		}
	}

	FlangerCol5(MapsLayout parent) {
		super(parent);
		addChild(new APTune(this));
	}
}
