package com.nonlinearlabs.client.world.maps.parameters.MonoAndUnison;

import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.BooleanControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.KnobLarge;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameterWithoutName;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;
import com.nonlinearlabs.client.world.maps.parameters.SmallModulateableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;
import com.nonlinearlabs.client.world.maps.parameters.UnModulateableParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

public class MonoColumn extends ParameterColumn {
    private final class GlideParameter extends ModulatableParameterWithoutName {
        private GlideParameter(MapsLayout parent, int parameterID) {
            super(parent, parameterID);
            addChild(new UnModulateableParameterName(this));
            addChild(new ModulationSourceLabel(this, getParameterNumber()));
            addChild(new KnobLarge(this, getParameterNumber()));
            addChild(new ValueDisplayLarge(this, getParameterNumber()));
        }
    }

    private class BooleanParameter extends Parameter {

        private BooleanParameter(MapsLayout parent, int id) {
            super(parent, id);
            addChild(new UnModulateableParameterName(this));

            addChild(new BooleanControlSmall(this, getParameterNumber()) {
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

        @Override
        protected int getBackgroundRoundings() {
            return Rect.ROUNDING_NONE;
        }

        @Override
        protected void startMouseEdit() {
            currentParameterChanger = EditBufferUseCases.get().startEditParameterValue(getParameterNumber(),
                    getPixRect().getWidth() / 2);
        }
    }

    public MonoColumn(ParameterGroup parent) {
        super(parent);
        addChild(new BooleanParameter(this, 364));
        addChild(new SmallModulateableParameter(this, 365));
        addChild(new GlideParameter(this, 367));
        addChild(new BooleanParameter(this, 366) {
            @Override
            protected int getBackgroundRoundings() {
                return Rect.ROUNDING_BOTTOM;
            }

            @Override
            protected void startMouseEdit() {
                currentParameterChanger = EditBufferUseCases.get().startEditParameterValue(getParameterNumber(), 200);
            }
        });
    }
}
