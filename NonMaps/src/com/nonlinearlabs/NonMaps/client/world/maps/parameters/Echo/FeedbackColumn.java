package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Echo;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class FeedbackColumn extends ParameterColumn {

	private class Feedback extends ModulateableKnob {

		private Feedback(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 229;
		}
	}

	private class CrossFB extends Parameter {

		private CrossFB(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 231;
		}
	}

	FeedbackColumn(MapsLayout parent) {
		super(parent);
		addChild(new Feedback(this));
		addChild(new CrossFB(this));
	}
}
