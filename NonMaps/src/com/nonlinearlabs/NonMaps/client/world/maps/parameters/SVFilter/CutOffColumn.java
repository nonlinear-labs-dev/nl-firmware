package com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class CutOffColumn extends ParameterColumn {

	private class CutOff extends ModulateableKnob {

		private CutOff(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 140;
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
			return 142;
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
			return 143;
		}
	}

	CutOffColumn(MapsLayout parent) {
		super(parent);
		addChild(new CutOff(this));
		addChild(new EnvC(this));
		addChild(new KeyTrk(this));
	}
}
