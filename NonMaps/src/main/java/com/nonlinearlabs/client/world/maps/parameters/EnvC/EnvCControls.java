package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;

class EnvCControls extends ParameterGroupControls {

	private class HorizontalBlock extends ParameterGroupControls {
		private HorizontalBlock(VerticalBlock parent) {
			super(parent);

			addChild(new EnvCCol2(this));
			addChild(new EnvCCol3(this));
			addChild(new EnvCCol4(this));
			addChild(new EnvCCol5(this));
		}

		@Override
		public double getXMargin() {
			return 0;
		}
	}

	private class VerticalBlock extends LayoutResizingVertical {
		private VerticalBlock(MapsLayout parent) {
			super(parent);

			addChild(new HorizontalBlock(this));
			addChild(new Spacer(this, 1, 45 / 2.0));
			addChild(new EnvCLevelVelLevelKTTimeKT(this));

		}
	}

	EnvCControls(MapsLayout parent) {
		super(parent);

		addChild(new EnvCCol1(this));
		addChild(new VerticalBlock(this));
		addChild(new EnvCCol6(this));
	}
}
