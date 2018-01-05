package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class FluctColumn extends ParameterColumn {

	private class Fluctuation extends ModulateableKnob {

		private Fluctuation(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 57;
		}
	}

	private class EnvelopeC extends ModulationSourceHighPriority {

		private EnvelopeC(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 59;
		}
	}

	private class Phase extends Parameter {

		private Phase(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 301;
		}
	}

	FluctColumn(MapsLayout parent) {
		super(parent);
		addChild(new Fluctuation(this));
		addChild(new EnvelopeC(this));
		addChild(new Phase(this));
	}
}
