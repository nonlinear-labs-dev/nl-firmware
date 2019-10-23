package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class DriveExtColumn extends ParameterColumn {

	private class Fold extends SmallKnobParameter {

		private Fold(MapsLayout parent) {
			super(parent, 74);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_RIGHT_TOP;
		}
	}

	private class Asym extends ParaSmallKnobParametermeter {

		private Asym(MapsLayout parent) {
			super(parent, 75);
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
