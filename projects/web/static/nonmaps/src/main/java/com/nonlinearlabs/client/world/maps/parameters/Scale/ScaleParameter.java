package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

public class ScaleParameter extends ModulationSourceHighPriority {

	public ScaleParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelModulationSource(this, getParameterNumber()) {
			@Override
			protected String getDisplayText() {
				return presenter.longName;
			}
		});
		addChild(new SliderHorizontal(this, getParameterNumber()));
		addChild(new ValueDisplaySmall(this, getParameterNumber()));
	}


	@Override
	protected void startMouseEdit() {
		currentParameterChanger = EditBufferUseCases.get().startEditParameterValue(getParameterNumber(),
				Millimeter.toPixels(800));
	}

	public double getDefaultValue() {
		return 0.0;
	}

	@Override
	public boolean skipChildOnLayout(MapsControl c)
	{
		return c instanceof ModulationSourceLabel;
	}

    @Override
    protected NonDimension layoutChildren(double levelOfDetail) {
        NonDimension ret = super.layoutChildren(levelOfDetail);

		for(MapsControl c: getChildren())
		{
			if(c instanceof ModulationSourceLabel)
			{
				double centeredX = (ret.getWidth() - c.getNonPosition().getWidth()) / 2.0;
				double height = c.getNonPosition().getHeight();
				c.moveTo(centeredX + getXMargin(), c.getNonPosition().getBottom() - height);	
			}
		}
        return ret;
    }

}