package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableSmallKnobParameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.UnmodulateableKnob;

class EnvAndPhaseColumn extends ParameterColumn {

    private class EnvF extends ModulateableKnob {

        private EnvF(MapsLayout parent) {
            super(parent, 307);
        }

        @Override
        protected int getBackgroundRoundings() {
            return Rect.ROUNDING_TOP;
        }
    }

    private class Phase extends ModulateableSmallKnobParameter {

        private Phase(MapsLayout parent) {
            super(parent, 213);
        }

        @Override
        protected int getBackgroundRoundings() {
            return Rect.ROUNDING_BOTTOM;
        }
    }

    public EnvAndPhaseColumn(MapsLayout parent) {
        super(parent);
        addChild(new EnvF(this));
        addChild(new Phase(this));
    }

}