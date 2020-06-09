package com.nonlinearlabs.client.world.maps.parameters.ShapeA;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.SizeLinkedParameterColumn;

public class AsymColumn extends SizeLinkedParameterColumn {

    private class Asym extends ModulateableKnob {

        private Asym(MapsLayout parent) {
            super(parent, 75);
        }
    }
    
    public AsymColumn(MapsLayout parent) {
        super(parent);
        addChild(new Asym(this));
    }
    
}