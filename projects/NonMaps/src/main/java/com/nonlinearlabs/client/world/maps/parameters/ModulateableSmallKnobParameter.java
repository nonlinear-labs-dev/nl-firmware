package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableSmallKnobParameter extends ModulatableParameterWithoutName {

    public ModulateableSmallKnobParameter(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
        addChild(new SmallParameterName(this, getParameterNumber()));
        addChild(new ModulationSourceLabel(this, getParameterNumber()));
        addChild(new KnobSmall(this, getParameterNumber()));
        addChild(new ValueDisplaySmall(this, getParameterNumber()));
    }

    @Override
	public boolean skipChildOnLayout(MapsControl c) {
		return c instanceof LabelModulationSource || c instanceof ModulationSourceLabel;
	}
}
