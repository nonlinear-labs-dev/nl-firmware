package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

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

	EnvACol1(MapsLayout parent) {
		super(parent);
		addChild(new Attack(this));
		addChild(new ModulationSourceSlider(this, 15));
		addChild(new SmallKnobParameter(this, 294));
	}
}
