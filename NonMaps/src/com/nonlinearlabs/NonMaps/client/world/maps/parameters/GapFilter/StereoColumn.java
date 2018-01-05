package com.nonlinearlabs.NonMaps.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class StereoColumn extends ParameterColumn {

	private class Center extends ModulateableKnob {

		private Center(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 201;
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
			return 203;
		}
	}

	StereoColumn(MapsLayout parent) {
		super(parent);
		addChild(new Center(this));
		addChild(new Stereo(this));
	}
}
