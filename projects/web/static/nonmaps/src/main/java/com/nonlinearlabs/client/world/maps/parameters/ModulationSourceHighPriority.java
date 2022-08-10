package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;

public abstract class ModulationSourceHighPriority extends ModulationSource {

	private ModulationSourceLabel modSrcLabel;

	public ModulationSourceHighPriority(MapsLayout parent, int parameterID) {
		super(parent, parameterID);

		if(presenter.modulation.isModulateable)
		{
			modSrcLabel = addChild(new ModulationSourceLabel(this, getParameterNumber()));
		}
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 4;
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
