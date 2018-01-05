package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class EnvACol1 extends ParameterColumn {

	private class Attack extends ModulateableKnob {

		private Attack(MapsLayout parent) {
			super(parent);

			// sorry for this one:
			// as the ModRouting controls register in the constructor and
			// their getParameterID() methods do not work at this time, they
			// will register as param 0
			// so there is code in Parameter() to not register params with
			// paramID 0
			// so the real param-0 has to register by itself
			getSelectionRoot().registerSelectable(this);
		}

		@Override
		public RGB getColorFontValueDisplay() {
			return ColorTable.getColorEnvOscAHeaderBackground();
		}

		@Override
		public int getParameterID() {
			return 0;
		}
	}

	private class Curve extends Parameter {

		private Curve(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 294;
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
			return 15;
		}
	}

	EnvACol1(MapsLayout parent) {
		super(parent);
		addChild(new Attack(this));
		addChild(new Velocity(this));
		addChild(new Curve(this));
	}
}
