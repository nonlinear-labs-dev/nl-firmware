package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class CombFilterCol3 extends ParameterColumn {

	private class Decay extends ModulateableKnob {

		private Decay(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 119;
		}
	}

	private class KeyTrk extends ModulationSourceHighPriority {

		private KeyTrk(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 121;
		}
	}

	private class Gate extends ModulateableKnob {

		private Gate(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 305;
		}
	}

	CombFilterCol3(MapsLayout parent) {
		super(parent);
		addChild(new Decay(this));
		addChild(new Gate(this));
		addChild(new KeyTrk(this));

	}
}
