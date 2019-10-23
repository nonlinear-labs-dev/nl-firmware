package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CombFilterCol5 extends ParameterColumn {

	private class APRes extends ModulateableKnob {

		private APRes(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 127;
		}
	}

	CombFilterCol5(MapsLayout parent) {
		super(parent);
		addChild(new APRes(this));
	}
}
