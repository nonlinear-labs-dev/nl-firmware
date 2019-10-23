package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class LevelColumn extends ParameterColumn {

	private class Level extends ModulatableSlider {

		private Level(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 185;
		}
	}

	private class KeyPan extends Parameter {

		private KeyPan(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 187;
		}
	}

	LevelColumn(MapsLayout parent) {
		super(parent);
		addChild(new Level(this));
		addChild(new KeyPan(this));
	}
}
