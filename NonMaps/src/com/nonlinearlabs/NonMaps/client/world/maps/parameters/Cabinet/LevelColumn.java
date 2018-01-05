package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class LevelColumn extends ParameterColumn {

	private class CabLevel extends ModulateableKnob {

		private CabLevel(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 197;
		}
	}

	LevelColumn(MapsLayout parent) {
		super(parent);
		addChild(new CabLevel(this));
	}
}
