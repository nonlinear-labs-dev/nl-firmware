package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class BColumn extends ParameterColumn {

	private class B extends ModulatableSlider {

		private B(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 172;
		}
	}

	private class Pan extends Parameter {

		private Pan(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 174;
		}
	}

	BColumn(MapsLayout parent) {
		super(parent);
		addChild(new B(this));
		addChild(new Pan(this));
	}
}
