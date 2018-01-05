package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Flanger;

import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.DoubleParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SizeLinkedParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.UnmodulateableKnob;

class FlangerCol6 extends DoubleParameterColumn {

	class FeedbackColumn extends SizeLinkedParameterColumn {

		private class Feedback extends ModulateableKnob {

			private Feedback(MapsLayout parent) {
				super(parent);
			}

			@Override
			public int getParameterID() {
				return 219;
			}

			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT;
			}
		}

		public FeedbackColumn(MapsLayout parent) {
			super(parent);
			addChild(new Feedback(this));
		}
	}

	class CrossFBAndHicut extends ParameterColumn {

		private class CrossFB extends UnmodulateableKnob {

			private CrossFB(MapsLayout parent) {
				super(parent);
			}

			@Override
			public int getParameterID() {
				return 221;
			}

			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_TOP;
			}
		}

		private class HiCut extends UnmodulateableKnob {

			private HiCut(MapsLayout parent) {
				super(parent);
			}

			@Override
			public int getParameterID() {
				return 222;
			}

			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_BOTTOM;
			}
		}

		public CrossFBAndHicut(MapsLayout parent) {
			super(parent);
			addChild(new CrossFB(this));
			addChild(new HiCut(this));
		}
	}

	FlangerCol6(MapsLayout parent) {
		super(parent);
		addChild(new FeedbackColumn(this));
		addChild(new CrossFBAndHicut(this));
	}
}
