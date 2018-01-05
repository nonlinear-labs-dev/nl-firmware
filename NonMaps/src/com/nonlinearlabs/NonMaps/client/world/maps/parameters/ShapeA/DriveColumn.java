package com.nonlinearlabs.NonMaps.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulateableKnob {

		private Drive(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 71;
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_TOP;
		}
	}

	private class EnvA extends ModulationSourceHighPriority {

		private EnvA(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 73;
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_BOTTOM;
		}
	}

	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new Drive(this));
		addChild(new EnvA(this));
	}
}
