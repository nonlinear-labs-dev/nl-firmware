package com.nonlinearlabs.client.world.maps.parameters.Master;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderVertical;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class MasterCol1 extends ParameterColumn {

	private class Volume extends Parameter {

		private Volume(MapsLayout parent) {
			super(parent, 247);
			addChild(new LabelLarge(this));
			addChild(new SliderVertical(this));
			addChild(new ValueDisplayLarge(this));
		}

	}

	private class Tune extends Parameter {

		private Tune(MapsLayout parent) {
			super(parent, 248);
			addChild(new LabelLarge(this));
			addChild(new NumericalControlSmall(this));
		}

	}

	MasterCol1(MapsLayout parent) {
		super(parent);
		addChild(new Volume(this));
		addChild(new Tune(this));
	}
}
