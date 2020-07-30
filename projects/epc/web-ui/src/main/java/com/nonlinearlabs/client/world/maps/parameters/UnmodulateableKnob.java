package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class UnmodulateableKnob extends Parameter {

	public UnmodulateableKnob(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new SmallParameterName(this, getParameterNumber()));
		addChild(new KnobSmall(this, getParameterNumber()));
		addChild(new ValueDisplaySmall(this, getParameterNumber()));
	}

}
