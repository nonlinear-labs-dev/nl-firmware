package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;
import com.nonlinearlabs.client.world.maps.parameters.SVFilter.LittleKnobSlider;

class CombFilterCol7 extends ParameterColumn {

	private class PMAB extends ModulationSourceHighPriority {

		private PMAB(MapsLayout parent) {
			super(parent, 135);
			addChild(new LabelModulationSource(this, getParameterNumber()));
			addChild(new LittleKnobSlider(this, getParameterNumber()));
			addChild(new ValueDisplaySmall(this, getParameterNumber()));
		}

	}

	CombFilterCol7(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 133));
		addChild(new PMAB(this));
	}
}
