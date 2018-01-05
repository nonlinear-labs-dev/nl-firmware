package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public abstract class UnmodulateableKnob extends Parameter {

	public UnmodulateableKnob(MapsLayout parent) {
		super(parent);
		addChild(new SmallParameterName(this, getName()));
		addChild(new KnobSmall(this));
		addChild(new ValueDisplaySmall(this));
	}

}
