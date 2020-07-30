package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public final class RingModKnob extends ModulatableParameter {
    public RingModKnob(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
        addChild(new ModulationSourceLabel(this, getParameterNumber()));
        addChild(new Spacer(this, 1, 23));
        addChild(new KnobLarge(this, getParameterNumber()));
        addChild(new Spacer(this, 1, 24));
        addChild(new ValueDisplayLarge(this, getParameterNumber()));
    }
}