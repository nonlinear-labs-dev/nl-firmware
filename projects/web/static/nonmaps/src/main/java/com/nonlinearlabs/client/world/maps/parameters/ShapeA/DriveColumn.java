package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnobWithSlidersHeight;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class DriveColumn extends ParameterColumn {

	private class EnvA extends ModulatableHorizontalSlider {

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
		addChild(new ModulateableKnobWithSlidersHeight(this, 71) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT_TOP | Rect.ROUNDING_RIGHT_TOP;
			}
		});
		addChild(new EnvA(this));
	}
} 
