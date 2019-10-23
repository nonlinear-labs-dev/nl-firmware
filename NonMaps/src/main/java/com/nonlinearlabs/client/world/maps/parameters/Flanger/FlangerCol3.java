package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class FlangerCol3 extends ParameterColumn {

	private class Time extends ModulateableKnob {

		private Time(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 216;
		}
	}

	private class Stereo extends Parameter {

		private Stereo(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 218;
		}
	}

	FlangerCol3(MapsLayout parent) {
		super(parent);
		addChild(new Time(this));
		addChild(new Stereo(this));
	}
}
