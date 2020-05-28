package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulatableHorizontalSlider extends ModulatableParameterWithoutName {

	public ModulatableHorizontalSlider(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulationSourceLabel(this, getParameterNumber()));
		addChild(new LabelModulationSource(this, getParameterNumber()));
		addChild(new ModulateableSliderHorizontal(this, getParameterNumber()));
		addChild(new ValueDisplayLarge(this, getParameterNumber()));
	}

	@Override
	public boolean skipChildOnLayout(MapsControl c) {
		return c instanceof LabelModulationSource;
	}

	
}
