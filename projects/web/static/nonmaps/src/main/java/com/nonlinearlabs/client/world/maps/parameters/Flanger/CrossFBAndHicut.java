package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class CrossFBAndHicut extends ParameterColumn {

    private class CrossFB extends ModulateableKnob {

        private CrossFB(MapsLayout parent) {
            super(parent, 221);
        }

        @Override
        protected int getBackgroundRoundings() {
            return Rect.ROUNDING_RIGHT_TOP;
        }
    }

    private class HiCut extends ModulateableKnob {

        private HiCut(MapsLayout parent) {
            super(parent, 222);
        }

        @Override
        protected int getBackgroundRoundings() {
            return Rect.ROUNDING_RIGHT_BOTTOM;
        }
    }

    public CrossFBAndHicut(MapsLayout parent) {
        super(parent);
        addChild(new CrossFB(this));
        addChild(new HiCut(this));
    }
}