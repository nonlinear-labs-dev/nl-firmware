package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class EnvCLevelVelLevelKTTimeKT extends ParameterGroupControls {

	private class LvlVel extends ModulationSourceSlider {

		private LvlVel(MapsLayout parent) {
			super(parent, 48);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

	private class LvlKT extends ModulationSourceSlider {

		private LvlKT(MapsLayout parent) {
			super(parent, 51);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

	private class TimeKT extends ModulationSourceSlider {

		private TimeKT(MapsLayout parent) {
			super(parent, 52);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

	EnvCLevelVelLevelKTTimeKT(MapsLayout parent) {
		super(parent);
		addChild(new LvlVel(this));
		addChild(new LvlKT(this));
		addChild(new TimeKT(this));
	}
}
