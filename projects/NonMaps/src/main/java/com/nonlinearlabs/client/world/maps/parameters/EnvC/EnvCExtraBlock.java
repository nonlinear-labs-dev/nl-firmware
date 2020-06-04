package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;

class EnvCExtraBlock extends ParameterGroupControls {

	private class Retrigger extends ModulationSourceSlider {

		private Retrigger(MapsLayout parent) {
			super(parent, 340);
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

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

	EnvCExtraBlock(MapsLayout parent) {
		super(parent);
		addChild(new Retrigger(this));
		addChild(new Spacer(this, 20, 1));
		addChild(new LvlVel(this));
		addChild(new LvlKT(this));
		addChild(new TimeKT(this));
	}
}
