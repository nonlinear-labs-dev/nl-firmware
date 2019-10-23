package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulateableKnob {

		private Drive(MapsLayout parent) {
			super(parent, 71);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_TOP;
		}
	}

	private class EnvA extends ModulationSourceSlider {

		private EnvA(MapsLayout parent) {
			super(parent, 73);
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
