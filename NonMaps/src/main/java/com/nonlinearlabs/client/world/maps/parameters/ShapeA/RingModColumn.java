package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class RingModColumn extends ParameterColumn {

	private class RingMod extends ModulateableKnob {

		private RingMod(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 81;
		}
	}

	RingModColumn(MapsLayout parent) {
		super(parent);
		addChild(new RingMod(this));
	}
}
