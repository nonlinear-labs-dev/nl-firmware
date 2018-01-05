package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class AColumn extends ParameterColumn {

	private class A extends ModulatableSlider {

		private A(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 169;
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
			return 171;
		}
	}

	AColumn(MapsLayout parent) {
		super(parent);
		addChild(new A(this));
		addChild(new Pan(this));
	}
}
