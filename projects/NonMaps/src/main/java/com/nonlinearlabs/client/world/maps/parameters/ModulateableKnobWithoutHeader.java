package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableKnobWithoutHeader extends ModulatableParameterWithoutName {

	public ModulateableKnobWithoutHeader(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelModulationSource(this, getParameterNumber()));
		addChild(new ModulationSourceLabel(this, getParameterNumber()));
		addChild(new KnobLarge(this, getParameterNumber()));
		addChild(new ValueDisplayLarge(this, getParameterNumber()));
	}
}
