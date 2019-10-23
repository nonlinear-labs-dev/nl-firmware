package com.nonlinearlabs.client.world.maps.parameters.OscA;

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

class PMFBColumn extends ParameterColumn {

	private class EnvC extends ModulationSourceHighPriority {

		private EnvC(MapsLayout parent) {
			super(parent, 70);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

	}

	private class Chirp extends Parameter {

		private Chirp(MapsLayout parent) {
			super(parent, 303);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

	}

	PMFBColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 68));
		addChild(new EnvC(this));
		addChild(new Chirp(this));
	}
}
