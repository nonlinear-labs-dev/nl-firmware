package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulateableKnob {

		private Drive(MapsLayout parent) {
			super(parent, 101);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_TOP;
		}
	}

	private class EnvB extends ModulationSourceHighPriority {

		private EnvB(MapsLayout parent) {
			super(parent, 103);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_LEFT_BOTTOM;
		}
	}

	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new Drive(this));
		addChild(new EnvB(this));
	}
}
