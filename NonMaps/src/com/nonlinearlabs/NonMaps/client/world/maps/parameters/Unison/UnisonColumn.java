package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Unison;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Spacer;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class UnisonColumn extends ParameterColumn {

	private class Voices extends Parameter {

		private Voices(MapsLayout parent) {
			super(parent);
			addChild(new LabelLarge(this, getName()));
			addChild(new NumericalControlSmall(this));
		}

		@Override
		public int getParameterID() {
			return 249;
		}
	}

	private class Detune extends ModulatableParameter {

		private Detune(MapsLayout parent) {
			super(parent);
			addChild(new ModulationSourceLabel(this));
			addChild(new NumericalControlSmall(this) {
				@Override
				protected double getInsetY() {
					return 0;
				}

				@Override
				protected double getBasicHeight() {
					return 14;
				}
			});

			addChild(new Spacer(this, 1, 6));
		}

		@Override
		public int getParameterID() {
			return 250;
		}
	}

	private class Phase extends ModulationSourceHighPriority {

		private Phase(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 252;
		}
	}

	private class Pan extends ModulationSourceHighPriority {

		private Pan(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 253;
		}
	}

	UnisonColumn(MapsLayout parent) {
		super(parent);
		addChild(new Voices(this));
		addChild(new Detune(this));
		addChild(new Phase(this));
		addChild(new Pan(this));
	}
}
