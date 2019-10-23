package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KeyTrackParameter;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableNumericalControl;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class CombFilterCol2 extends ParameterColumn {

	private class Pitch extends ModulatableParameter {

		private Pitch(MapsLayout parent) {
			super(parent);
			addChild(new ModulateableNumericalControl(this));
		}

		@Override
		public int getParameterID() {
			return 115;
		}

		@Override
		public double getPadding() {
			return 2;
		}
	}

	private class EnvC extends ModulationSourceHighPriority {

		private EnvC(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 118;
		}
	}

	private class Keytracking extends ModulationSourceHighPriority {

		private Keytracking(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new KeyTrackParameter(this));
		}

		@Override
		public int getParameterID() {
			return 117;
		}
	}

	CombFilterCol2(MapsLayout parent) {
		super(parent);
		addChild(new Pitch(this));
		addChild(new EnvC(this));
		addChild(new Keytracking(this));

	}
}
