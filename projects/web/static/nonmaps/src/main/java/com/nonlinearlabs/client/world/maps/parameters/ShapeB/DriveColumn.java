package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnobWithSlidersHeight;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class DriveColumn extends ParameterColumn {

	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnobWithSlidersHeight(this, 101) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT_TOP | Rect.ROUNDING_RIGHT_TOP;
			}
		});

		addChild(new ModulatableHorizontalSlider(this, 103) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT_BOTTOM | Rect.ROUNDING_RIGHT_BOTTOM;
			}
		});
	}
}
