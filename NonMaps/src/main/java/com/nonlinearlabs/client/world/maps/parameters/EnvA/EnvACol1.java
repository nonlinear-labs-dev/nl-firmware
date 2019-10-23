package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class EnvACol1 extends ParameterColumn {

	private class Attack extends ModulateableKnob {

		private Attack(MapsLayout parent) {
			super(parent, 0);
		}

		@Override
		public RGB getColorFontValueDisplay() {
			return ColorTable.getColorEnvOscAHeaderBackground();
		}
	}

	private class Curve extends Parameter {

		private Curve(MapsLayout parent) {
			super(parent, 294);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	private class Velocity extends ModulationSourceHighPriority {

		private Velocity(MapsLayout parent) {
			super(parent, 15);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	EnvACol1(MapsLayout parent) {
		super(parent);
		addChild(new Attack(this));
		addChild(new Velocity(this));
		addChild(new Curve(this));
	}
}
