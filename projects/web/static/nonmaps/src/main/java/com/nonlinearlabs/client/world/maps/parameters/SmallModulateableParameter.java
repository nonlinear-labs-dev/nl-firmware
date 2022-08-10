package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;

public class SmallModulateableParameter extends ModulatableParameterWithoutName {

    private ModulationSourceLabel modSrcLabel;

    public SmallModulateableParameter(MapsLayout parent, int num) {
        super(parent, num);
        addChild(new UnModulateableParameterName(this) {
            @Override
            public double getLevelOfDetailForFullVisibility() {
                return getParent().getLevelOfDetailForFullVisibility() + 1;
            }
        });

        modSrcLabel = addChild(new ModulationSourceLabel(this, getParameterNumber()) {
            @Override
            public double getLevelOfDetailForFullVisibility() {
                return 0;
            }
        });

        addChild(new NumericalControlSmall(this, getParameterNumber()) {
            @Override
            protected double getInsetY() {
                return 0;
            }

            @Override
            protected double getBasicHeight() {
                return 14;
            }

            @Override
            protected double getMinHeight() {
                return 0;
            }

            @Override
            public double getLevelOfDetailForFullVisibility() {
                return getParent().getLevelOfDetailForFullVisibility() + 1;
            }
        });

        addChild(new Spacer(this, 1, 6));
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