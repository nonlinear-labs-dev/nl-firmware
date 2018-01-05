package com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class LBHColumn extends ParameterColumn {

	private class LBH extends ModulateableKnob {

		private LBH(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 150;
		}
	}

	private class Parallel extends Parameter {

		private Parallel(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 152;
		}
	}

	LBHColumn(MapsLayout parent) {
		super(parent);
		addChild(new LBH(this));
		addChild(new Parallel(this));
	}
}
