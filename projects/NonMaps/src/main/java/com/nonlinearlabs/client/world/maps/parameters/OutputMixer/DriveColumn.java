package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobLarge;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

class DriveColumn extends ParameterColumn {

	private class Drive extends ModulatableParameter {

		private Drive(MapsLayout parent) {
			super(parent, 181);

			addChild(new ModulationSourceLabel(this, getParameterNumber()));
			addChild(new Spacer(this, 1, 23));
			addChild(new KnobLarge(this, getParameterNumber()));
			addChild(new Spacer(this, 1, 24));
			addChild(new ValueDisplayLarge(this, getParameterNumber()));
		}
	}

 


	DriveColumn(MapsLayout parent) {
		super(parent);
		addChild(new Drive(this));
	}
}
