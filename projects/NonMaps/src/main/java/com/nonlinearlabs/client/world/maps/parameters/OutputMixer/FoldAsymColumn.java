package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FoldAsymColumn extends ParameterColumn {

    public FoldAsymColumn(MapsLayout parent) {
        super(parent);
        addChild(new SmallKnobParameter(this, 183));
        addChild(new SmallKnobParameter(this, 184));
    }
}