package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableKnobWithSlidersHeight extends ModulatableParameter {

    public ModulateableKnobWithSlidersHeight(MapsLayout parent, int parameterNumber) {
        super(parent, parameterNumber);

        addChild(new ModulationSourceLabel(this, getParameterNumber()));
        addChild(new Spacer(this, 1, 23.5));
        addChild(new KnobLarge(this, getParameterNumber()));
        addChild(new ValueDisplayLarge(this, getParameterNumber()));
        addChild(new Spacer(this, 1, 23.5));
    }

}