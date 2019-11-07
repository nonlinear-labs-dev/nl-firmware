package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulateableKnob {

		private Drive(MapsLayout parent) {
			super(parent, 188);
			insertChild(1, new Spacer(this, 1, 23) {
				@Override
				public double getLevelOfDetailForFullVisibility() {
					return 3;
				}
			});

			addChild(new Spacer(this, 1, 24) {
				@Override
				public double getLevelOfDetailForFullVisibility() {
					return 3;
				}
			});
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT;
		}
	}

	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new Drive(this));
	}
}
