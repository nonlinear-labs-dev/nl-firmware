package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.DoubleParameterColumn;

public class FBMixAndRingModColumns extends DoubleParameterColumn {

    public FBMixAndRingModColumns(MapsLayout parent) {
        super(parent);
        FBMixColumn col = addChild(new FBMixColumn(this));
        addChild(new RingModColumn(this, col.getSlider()));
    }
}