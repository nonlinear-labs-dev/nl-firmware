package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FoldAsymColumn extends ParameterColumn {

    public FoldAsymColumn(MapsLayout parent) {
        super(parent);
        addChild(new SmallKnobParameter(this, 183) {
            @Override
            protected int getBackgroundRoundings() {
                return Rect.ROUNDING_RIGHT_TOP;
            }
        });

        addChild(new SmallKnobParameter(this, 184) {
            @Override
            protected int getBackgroundRoundings() {
                return Rect.ROUNDING_RIGHT_BOTTOM;
            }
        });
    }

    @Override
    protected boolean shouldDisplayHeaderOnFirstParameter() {
        return false;
    }
}