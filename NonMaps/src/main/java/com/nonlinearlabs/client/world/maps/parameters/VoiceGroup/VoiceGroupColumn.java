package com.nonlinearlabs.client.world.maps.parameters.VoiceGroup;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.UnModulateableParameterName;

public class VoiceGroupColumn extends ParameterColumn {
    private class Volume extends Parameter {

        private Volume(MapsLayout parent) {
            super(parent, 249);
            addChild(new UnModulateableParameterName(this));
            addChild(new NumericalControlSmall(this, getParameterNumber()));
        }
    }

    private class Tune extends Parameter {

        private Tune(MapsLayout parent) {
            super(parent, 249);
            addChild(new UnModulateableParameterName(this));
            addChild(new NumericalControlSmall(this, getParameterNumber()));
        }

    }

    public VoiceGroupColumn(MapsLayout parent) {
        super(parent);
        addChild(new Volume(this));
        addChild(new Tune(this));
    }

}
