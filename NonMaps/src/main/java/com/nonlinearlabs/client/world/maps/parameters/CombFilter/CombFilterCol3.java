package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class CombFilterCol3 extends ParameterColumn {

	private class KeyTrk extends ModulationSourceHighPriority {

		private KeyTrk(MapsLayout parent) {
			super(parent, 121);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	CombFilterCol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 119));
		addChild(new ModulateableKnob(this, 305));
		addChild(new KeyTrk(this));

	}
}
