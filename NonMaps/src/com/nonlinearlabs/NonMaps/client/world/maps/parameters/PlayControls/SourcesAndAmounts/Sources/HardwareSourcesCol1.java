package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;

public class HardwareSourcesCol1 extends ParameterColumn {

	private class Pedal1 extends Pedal {

		private Pedal1(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 254;
		}
	}

	private class Pedal2 extends Pedal {

		private Pedal2(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 259;
		}
	}

	private class Pedal3 extends Pedal {

		private Pedal3(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 264;
		}
	}

	private class Pedal4 extends Pedal {

		private Pedal4(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 269;
		}
	}

	private class Pitchbend extends PhysicalControlParameter {

		private Pitchbend(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 274;
		}

		@Override
		public ReturnMode getReturnMode() {
			return ReturnMode.Center;
		}
	}

	private class Aftertouch extends PhysicalControlParameter {

		private Aftertouch(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 279;
		}

		@Override
		public ReturnMode getReturnMode() {
			return ReturnMode.Zero;
		}
	}

	private class Ribbon1 extends Ribbon {

		private Ribbon1(HardwareSourcesCol1 parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 284;
		}
	}

	private class Ribbon2 extends Ribbon {

		private Ribbon2(HardwareSourcesCol1 parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 289;
		}
	}

	private Pedal[] pedal = new Pedal[4];

	public HardwareSourcesCol1(MapsLayout parent) {
		super(parent);
		addChild(pedal[0] = new Pedal1(this) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT_TOP;
			}
		});
		addChild(pedal[1] = new Pedal2(this));
		addChild(pedal[2] = new Pedal3(this));
		addChild(pedal[3] = new Pedal4(this));
		addChild(new Pitchbend(this));
		addChild(new Aftertouch(this));
		addChild(new Ribbon1(this));
		addChild(new Ribbon2(this) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_LEFT_BOTTOM;
			}
		});
	}

	public Pedal getPedal(int i) {
		return pedal[i];
	}
}
