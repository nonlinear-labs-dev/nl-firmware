package com.nonlinearlabs.client.world.maps.parameters.Voices;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;

public class UnisonColumn extends ParameterColumn {

    private class Voices extends Parameter {

        private Voices(MapsLayout parent) {
            super(parent, 249);
            addChild(new LabelLarge(this));
            addChild(new NumericalControlSmall(this, getParameterID()));
        }

    }

    private class Detune extends ModulatableParameter {

        private Detune(MapsLayout parent) {
            super(parent, 250);
            addChild(new ModulationSourceLabel(this, getParameterID()));
            addChild(new NumericalControlSmall(this, getParameterID()) {
                @Override
                protected double getInsetY() {
                    return 0;
                }

                @Override
                protected double getBasicHeight() {
                    return 14;
                }
            });

            addChild(new Spacer(this, 1, 6));
        }

    }

    public UnisonColumn(MapsLayout parent) {
        super(parent);
        addChild(new Voices(this));
        addChild(new Detune(this));
        addChild(new ModulationSourceSlider(this, 252));
        addChild(new ModulationSourceSlider(this, 253));
    }

}
