package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class FlangerCol2 extends ParameterColumn {

	private class TimeMod extends ModulateableKnob {

		private TimeMod(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 211;
		}
	}

	FlangerCol2(MapsLayout parent) {
		super(parent);
		addChild(new TimeMod(this));
	}
}
