package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnobWithSlidersHeight;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

public class AsymColumn extends ParameterColumn {

    public AsymColumn(MapsLayout parent) {
        super(parent);
        addChild(new ModulateableKnobWithSlidersHeight(this, 105));
    }
}