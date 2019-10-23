package com.nonlinearlabs.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class BalanceColumn extends ParameterColumn {

	private class Balance extends ModulateableKnob {

		private Balance(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 207;
		}
	}

	BalanceColumn(MapsLayout parent) {
		super(parent);
		addChild(new Balance(this));
	}
}
