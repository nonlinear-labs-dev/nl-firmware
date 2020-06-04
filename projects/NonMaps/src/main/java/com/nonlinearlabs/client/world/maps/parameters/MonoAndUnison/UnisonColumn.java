package com.nonlinearlabs.client.world.maps.parameters.MonoAndUnison;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameterWithoutName;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;
import com.nonlinearlabs.client.world.maps.parameters.UnModulateableParameterName;

public class UnisonColumn extends ParameterColumn {
    private class Voices extends Parameter {

        private Voices(MapsLayout parent) {
            super(parent, 249);
            addChild(new UnModulateableParameterName(this));
            addChild(new NumericalControlSmall(this, getParameterNumber()) {
                @Override
                protected double getInsetY() {
                    return 2;
                }

                @Override
                protected double getBasicHeight() {
                    return 19;
                }
            });
        }

        @Override
        protected int getBackgroundRoundings() {
            return Rect.ROUNDING_NONE;
        }
    }

    private class Detune extends ModulatableParameterWithoutName {

        private ModulationSourceLabel modSrcLabel;

        private Detune(MapsLayout parent) {
            super(parent, 250);
            addChild(new UnModulateableParameterName(this));
            modSrcLabel = addChild(new ModulationSourceLabel(this, getParameterNumber()));
            addChild(new NumericalControlSmall(this, getParameterNumber()) {
                @Override
                protected double getInsetY() {
                    return 3;
                }

                @Override
                protected double getBasicHeight() {
                    return 19;
                }
            });
        }

        @Override
        public boolean skipChildOnLayout(MapsControl c) {
            return c instanceof ModulationSourceLabel;
        }

        @Override
        protected NonDimension layoutChildren(double levelOfDetail) {
            NonDimension ret = super.layoutChildren(levelOfDetail);
            double centeredX = (ret.getWidth() - modSrcLabel.getNonPosition().getWidth()) / 2.0;
            modSrcLabel.moveTo(centeredX + getXMargin(), modSrcLabel.getNonPosition().getTop());
            return ret;
        }

    }

    public UnisonColumn(ParameterGroup parent) {
        super(parent);
        addChild(new Voices(this));
        addChild(new Detune(this));
        addChild(new ModulationSourceSlider(this, 252));
        addChild(new ModulationSourceSlider(this, 253));
    }

}
