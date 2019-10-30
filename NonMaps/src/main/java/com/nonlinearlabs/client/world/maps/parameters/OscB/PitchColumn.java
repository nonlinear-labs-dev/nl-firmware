package com.nonlinearlabs.client.world.maps.parameters.OscB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KeyTrackParameter;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableNumericalControl;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class PitchColumn extends ParameterColumn {

	private class Pitch extends ModulatableParameter {

		private Pitch(MapsLayout parent) {
			super(parent, 83);
			addChild(new ModulateableNumericalControl(this, getParameterID()));
		}

		@Override
		public double getPadding() {
			return 2;
		}
	}

	private class Keytracking extends ModulationSourceHighPriority {

		private Keytracking(MapsLayout parent) {
			super(parent, 85);
			addChild(new LabelModulationSource(this, getParameterID()));
			addChild(new KeyTrackParameter(this, getParameterID()));
		}

	}

	PitchColumn(MapsLayout parent) {
		super(parent);
		addChild(new Pitch(this));
		addChild(new ModulationSourceSlider(this, 86));
		addChild(new Keytracking(this));
	}
}
