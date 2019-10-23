package com.nonlinearlabs.client.world.maps.parameters.Reverb;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class SizeColumn extends ParameterColumn {

	private class Size extends ModulateableKnob {

		private Size(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 235;
		}
	}

	private class Chorus extends Parameter {

		private Chorus(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 237;
		}
	}

	SizeColumn(MapsLayout parent) {
		super(parent);
		addChild(new Size(this));
		addChild(new Chorus(this));
	}
}
