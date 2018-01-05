package com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class FMColumn extends ParameterColumn {

	private class FM extends ModulateableKnob {

		private FM(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 153;
		}
	}

	private class AB extends ModulationSourceHighPriority {

		private AB(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new LittleKnobSlider(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public boolean shouldHaveHandleOnly() {
			return true;
		}

		@Override
		public int getParameterID() {
			return 155;
		}
	}

	FMColumn(MapsLayout parent) {
		super(parent);
		addChild(new FM(this));
		addChild(new AB(this));
	}
}
