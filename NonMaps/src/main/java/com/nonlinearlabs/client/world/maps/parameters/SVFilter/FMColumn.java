package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class FMColumn extends ParameterColumn {

	private class AB extends ModulationSourceHighPriority {

		private AB(MapsLayout parent) {
			super(parent, 155);
			addChild(new LabelModulationSource(this));
			addChild(new LittleKnobSlider(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public boolean shouldHaveHandleOnly() {
			return true;
		}

	}

	FMColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 153));
		addChild(new AB(this));
	}
}
