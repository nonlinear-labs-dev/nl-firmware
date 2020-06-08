package com.nonlinearlabs.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.DoubleParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SizeLinkedParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.UnmodulateableKnob;

class FlangerCol1 extends DoubleParameterColumn {

	private class RateColumn extends SizeLinkedParameterColumn {

		private class Rate extends ModulateableKnob {

			private Rate(MapsLayout parent) {
				super(parent, 214);
			}

			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT;
			}
		}

		public RateColumn(FlangerCol1 parent) {
			super(parent);
			addChild(new Rate(this));
		}
	}

	private class EnvAndPhaseColumn extends ParameterColumn {

		private class EnvF extends UnmodulateableKnob {

			private EnvF(MapsLayout parent) {
				super(parent, 307);
			}

			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_TOP;
			}
		}

		private class Phase extends UnmodulateableKnob {

			private Phase(MapsLayout parent) {
				super(parent, 213);
			}

			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_BOTTOM;
			}
		}

		public EnvAndPhaseColumn(FlangerCol1 parent) {
			super(parent);
			addChild(new EnvF(this));
			addChild(new Phase(this));
		}

	}

	FlangerCol1(MapsLayout parent) {
		super(parent);
		RateColumn f = addChild(new RateColumn(this));
		EnvAndPhaseColumn e = addChild(new EnvAndPhaseColumn(this));
		f.setReferenceControl(e);
	}
}
