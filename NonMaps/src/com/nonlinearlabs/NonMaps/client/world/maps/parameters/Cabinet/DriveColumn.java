package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Spacer;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulateableKnob {

		private Drive(MapsLayout parent) {
			super(parent);
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
		public int getParameterID() {
			return 188;
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
