package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnobWithSlidersHeight;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class DriveColumn extends ParameterColumn {

	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnobWithSlidersHeight(this, 188) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT;
			}
		});
	}
}
