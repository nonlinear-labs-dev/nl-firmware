package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB;

import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class EnvBLevelVelLevelKTTimeKT extends ParameterGroupControls {

	private class LvlVel extends ModulationSourceHighPriority {

		private LvlVel(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 33;
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

	private class LvlKT extends ModulationSourceHighPriority {

		private LvlKT(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 36;
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

	private class TimeKT extends ModulationSourceHighPriority {

		private TimeKT(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 37;
		}

		@Override
		protected int getBackgroundRoundings() {
			return Rect.ROUNDING_ALL;
		}
	}

	EnvBLevelVelLevelKTTimeKT(MapsLayout parent) {
		super(parent);
		addChild(new LvlVel(this));
		addChild(new LvlKT(this));
		addChild(new TimeKT(this));
	}
}
