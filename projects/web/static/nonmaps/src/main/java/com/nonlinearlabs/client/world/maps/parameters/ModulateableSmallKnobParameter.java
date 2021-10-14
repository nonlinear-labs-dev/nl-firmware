package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;

public class ModulateableSmallKnobParameter extends ModulatableParameterWithoutName {

    private ModulationSourceLabel modSrcLabel;

    public ModulateableSmallKnobParameter(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
        addChild(new SmallParameterName(this, getParameterNumber()));
        modSrcLabel = addChild(new ModulationSourceLabel(this, getParameterNumber()));
        addChild(new KnobSmall(this, getParameterNumber()));
        addChild(new ValueDisplaySmall(this, getParameterNumber()));
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
