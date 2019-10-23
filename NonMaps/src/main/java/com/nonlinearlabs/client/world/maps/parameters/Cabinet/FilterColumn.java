package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class FilterColumn extends ParameterColumn {

	private class HiCut extends ModulateableKnob {

		private HiCut(MapsLayout parent) {
			super(parent, 194);
		}
	}

	private class LoCut extends Parameter {

		private LoCut(MapsLayout parent) {
			super(parent, 196);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	FilterColumn(MapsLayout parent) {
		super(parent);
		addChild(new HiCut(this));
		addChild(new LoCut(this));
	}
}
