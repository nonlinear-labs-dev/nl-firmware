package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class EnvCCol6 extends ParameterColumn {

	private class Release extends ModulateableKnob {

		private Release(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 46;
		}
	}

	private class Velocity extends ModulationSourceHighPriority {

		private Velocity(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 50;
		}
	}

	EnvCCol6(MapsLayout parent) {
		super(parent);
		addChild(new Release(this));
		addChild(new Velocity(this));
	}
}
