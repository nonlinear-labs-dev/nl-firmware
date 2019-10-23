package com.nonlinearlabs.client.world.maps.parameters.Echo;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class TimeColumn extends ParameterColumn {

	private class Time extends ModulateableKnob {

		private Time(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 225;
		}
	}

	TimeColumn(MapsLayout parent) {
		super(parent);
		addChild(new Time(this));
	}
}
