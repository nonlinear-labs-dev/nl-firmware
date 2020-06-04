package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobLarge;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class RingModColumn extends ParameterColumn {

	private final class RingModKnob extends ModulatableParameter {
		private RingModKnob(MapsLayout parent, int parameterID) {
			super(parent, parameterID);
			addChild(new ModulationSourceLabel(this, getParameterNumber()));
			addChild(new Spacer(this, 1, 23));
			addChild(new KnobLarge(this, getParameterNumber()));
			addChild(new Spacer(this, 1, 24));
			addChild(new ValueDisplayLarge(this, getParameterNumber()));
		}
	}

	RingModColumn(MapsLayout parent) {
		super(parent);
		addChild(new RingModKnob(this, 111));
	}

}
