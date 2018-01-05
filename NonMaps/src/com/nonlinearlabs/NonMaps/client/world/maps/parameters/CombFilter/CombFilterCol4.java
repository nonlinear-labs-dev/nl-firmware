package com.nonlinearlabs.NonMaps.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class CombFilterCol4 extends ParameterColumn {

	private class APTune extends ModulateableKnob {

		private APTune(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 123;
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
			return 125;
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
			return 126;
		}
	}

	CombFilterCol4(MapsLayout parent) {
		super(parent);
		addChild(new APTune(this));
		addChild(new EnvC(this));
		addChild(new KeyTrk(this));

	}
}
