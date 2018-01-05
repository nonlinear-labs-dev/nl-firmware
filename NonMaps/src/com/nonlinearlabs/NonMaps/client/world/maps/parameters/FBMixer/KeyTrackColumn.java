package com.nonlinearlabs.NonMaps.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class KeyTrackColumn extends ParameterColumn {

	private class Level extends ModulatableSlider {

		private Level(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 299;
		}
	}

	private class KeyTrack extends ModulationSourceHighPriority {

		private KeyTrack(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 168;
		}
	}

	KeyTrackColumn(MapsLayout parent) {
		super(parent);
		addChild(new Level(this));
		addChild(new KeyTrack(this));
	}
}
