package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;

public class SmallKnobParameter extends ModulatableParameter {

    private ModulationSourceLabel modSrcLabel;

    public SmallKnobParameter(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
        if(presenter.modulation.isModulateable)
		{
			modSrcLabel = addChild(new ModulationSourceLabel(this, getParameterNumber()) {
				@Override
				public double getLevelOfDetailForFullVisibility() {
					return getParent().getLevelOfDetailForFullVisibility() + 1;
				}
			});
		}
        addChild(new SmallParameterName(this, getParameterNumber()));
        addChild(new KnobSmall(this, getParameterNumber()));
        addChild(new ValueDisplaySmall(this, getParameterNumber()));
    }

    @Override
    public boolean skipChildOnLayout(MapsControl c) {
		if(modSrcLabel == null)
			return false;
		else
        	return c instanceof ModulationSourceLabel;
    }

    @Override
    protected NonDimension layoutChildren(double levelOfDetail) {
        NonDimension ret = super.layoutChildren(levelOfDetail);
		if(modSrcLabel != null)
		{
			double centeredX = (ret.getWidth() - modSrcLabel.getNonPosition().getWidth()) / 2.0;
			modSrcLabel.moveTo(centeredX + getXMargin(), modSrcLabel.getNonPosition().getTop());	
		}
		return ret;
    }
}
