package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobLarge;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulatableParameter {

		private Drive(MapsLayout parent) {
			super(parent, 71);

			addChild(new ModulationSourceLabel(this, getParameterNumber()));
			addChild(new Spacer(this, 1, 23));
			addChild(new KnobLarge(this, getParameterNumber()));
			addChild(new Spacer(this, 1, 24));
			addChild(new ValueDisplayLarge(this, getParameterNumber()));
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_TOP | Rect.ROUNDING_RIGHT_TOP;
		}
	}

	private class EnvA extends ModulationSourceSlider {

		private EnvA(MapsLayout parent) {
			super(parent, 73);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_BOTTOM | Rect.ROUNDING_RIGHT_BOTTOM;
		}
	}

	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new Drive(this));
		addChild(new EnvA(this));
	}
}
