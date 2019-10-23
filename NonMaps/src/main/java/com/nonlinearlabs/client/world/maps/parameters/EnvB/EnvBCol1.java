package com.nonlinearlabs.client.world.maps.parameters.EnvB;

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

class EnvBCol1 extends ParameterColumn {

	private class Curve extends Parameter {

		private Curve(MapsLayout parent) {
			super(parent, 295);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	private class Velocity extends ModulationSourceHighPriority {

		private Velocity(MapsLayout parent) {
			super(parent, 34);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	EnvBCol1(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 19));
		addChild(new Velocity(this));
		addChild(new Curve(this));
	}
}
