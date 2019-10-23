package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class UnmodulateableKnob extends Parameter {

	public UnmodulateableKnob(MapsLayout parent) {
		super(parent);
		addChild(new SmallParameterName(this, getName()));
		addChild(new KnobSmall(this));
		addChild(new ValueDisplaySmall(this));
	}

}
