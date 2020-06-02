package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.TripleParameterColumn;

public class FoldAsymMixColumn extends TripleParameterColumn {

    public FoldAsymMixColumn(MapsLayout parent) {
        super(parent);
        addChild(new DriveExtColumn(this));
        addChild(new AsymColumn(this));
        addChild(new MixColumn(this));
    }
    
}