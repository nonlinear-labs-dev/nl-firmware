package com.nonlinearlabs.client.world.maps.parameters.Master;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameterWithoutName;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderVertical;
import com.nonlinearlabs.client.world.maps.parameters.SmallModulateableParameter;
import com.nonlinearlabs.client.world.maps.parameters.UnModulateableParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class MasterCol1 extends ParameterColumn {

	private final class MasterVolume extends ModulatableParameterWithoutName {
        private MasterVolume(MapsLayout parent, int parameterID) {
            super(parent, parameterID);
            addChild(new UnModulateableParameterName(this));
            addChild(new ModulationSourceLabel(this, getParameterNumber()));
            addChild(new SliderVertical(this, getParameterNumber()));
            addChild(new ValueDisplayLarge(this, getParameterNumber()));
        }
    }

	MasterCol1(MapsLayout parent) {
		super(parent);
		addChild(new MasterVolume(this, 247));
		addChild(new SmallModulateableParameter(this, 248));
        addChild(new ModulationSourceSlider(this, 408));
		addChild(new ModulationSourceSlider(this, 406));
	}
}
