package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class DriveExtColumn extends ParameterColumn {

	private class Fold extends Parameter {

		private Fold(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 104;
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_RIGHT_TOP;
		}
	}

	private class Asym extends Parameter {

		private Asym(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 105;
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
