package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class CutOffColumn extends ParameterColumn {

	private class KeyTrk extends ModulationSourceHighPriority {

		private KeyTrk(MapsLayout parent) {
			super(parent, 142);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

	}

	private class EnvC extends ModulationSourceHighPriority {

		private EnvC(MapsLayout parent) {
			super(parent, 143);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

	}

	CutOffColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 140));
		addChild(new EnvC(this));
		addChild(new KeyTrk(this));
	}
}
