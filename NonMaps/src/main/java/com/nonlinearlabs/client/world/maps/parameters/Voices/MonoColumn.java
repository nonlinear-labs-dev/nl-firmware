package com.nonlinearlabs.client.world.maps.parameters.Voices;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

public class MonoColumn extends ParameterColumn {
    private class Voices extends Parameter {

        private Voices(MapsLayout parent) {
            super(parent, 249);
            addChild(new LabelLarge(this, getName()));
            addChild(new NumericalControlSmall(this));
        }

    }

    private class Detune extends ModulatableParameter {

        private Detune(MapsLayout parent) {
            super(parent, 250);
            addChild(new ModulationSourceLabel(this));
            addChild(new NumericalControlSmall(this) {
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

    private class Phase extends ModulationSourceHighPriority {

        private Phase(MapsLayout parent) {
            super(parent, 252);
            addChild(new LabelModulationSource(this, getName()));
            addChild(new SliderHorizontal(this));
            addChild(new ValueDisplaySmall(this));
        }

    }

    private class Pan extends ModulationSourceHighPriority {

        private Pan(MapsLayout parent) {
            super(parent, 253);
            addChild(new LabelModulationSource(this, getName()));
            addChild(new SliderHorizontal(this));
            addChild(new ValueDisplaySmall(this));
        }

    }

    public MonoColumn(MapsLayout parent) {
        super(parent);
        addChild(new Voices(this));
        addChild(new Detune(this));
        addChild(new Phase(this));
        addChild(new Pan(this));
    }

}
