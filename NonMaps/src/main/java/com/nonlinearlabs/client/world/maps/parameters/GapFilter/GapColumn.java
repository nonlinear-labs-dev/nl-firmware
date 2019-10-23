package com.nonlinearlabs.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class GapColumn extends ParameterColumn {

	private class Gap extends ModulateableKnob {

		private Gap(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 204;
		}
	}

	private class Resonance extends Parameter {

		private Resonance(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 206;
		}
	}

	GapColumn(MapsLayout parent) {
		super(parent);
		addChild(new Gap(this));
		addChild(new Resonance(this));
	}
}
