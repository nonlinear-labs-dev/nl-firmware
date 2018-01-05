package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Reverb;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class FilterColumn extends ParameterColumn {

	private class HiCut extends ModulateableKnob {

		private HiCut(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 238;
		}
	}

	private class LoCut extends Parameter {

		private LoCut(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 240;
		}

	}

	FilterColumn(MapsLayout parent) {
		super(parent);
		addChild(new HiCut(this));
		addChild(new LoCut(this));
	}
}
