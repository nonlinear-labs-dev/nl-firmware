package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Master;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplayLarge;

class MasterCol1 extends ParameterColumn {

	private class Volume extends Parameter {

		private Volume(MapsLayout parent) {
			super(parent);
			addChild(new LabelLarge(this, getName()));
			addChild(new SliderVertical(this));
			addChild(new ValueDisplayLarge(this));
		}

		@Override
		public int getParameterID() {
			return 247;
		}
	}

	private class Tune extends Parameter {

		private Tune(MapsLayout parent) {
			super(parent);
			addChild(new LabelLarge(this, getName()));
			addChild(new NumericalControlSmall(this));
		}

		@Override
		public int getParameterID() {
			return 248;
		}
	}

	MasterCol1(MapsLayout parent) {
		super(parent);
		addChild(new Volume(this));
		addChild(new Tune(this));
	}
}
