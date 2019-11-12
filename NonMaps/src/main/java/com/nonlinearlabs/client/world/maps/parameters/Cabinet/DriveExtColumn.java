package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class DriveExtColumn extends ParameterColumn {

	private class Fold extends SmallKnobParameter {

		private Fold(MapsLayout parent) {
			super(parent, 190);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_RIGHT_TOP;
		}
	}

	private class Asym extends SmallKnobParameter {

		private Asym(MapsLayout parent) {
			super(parent, 191);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_RIGHT_BOTTOM;
		}
	}

	DriveExtColumn(MapsLayout parent) {
		super(parent);
		addChild(new Fold(this));
		addChild(new Asym(this));
	}
}
