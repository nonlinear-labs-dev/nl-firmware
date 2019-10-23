package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol4 extends AmountsCol {

	public AmountsCol4(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 258, 254, 246) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_TOP;
			}
		});
		addChild(new ModulationRoutingParameter(this, 263, 259, 246));
		addChild(new ModulationRoutingParameter(this, 268, 264, 246));
		addChild(new ModulationRoutingParameter(this, 273, 269, 246));
		addChild(new ModulationRoutingParameter(this, 278, 274, 246));
		addChild(new ModulationRoutingParameter(this, 283, 279, 246));
		addChild(new RibbonModulationRoutingParameter(this, 288, 284, 246));
		addChild(new RibbonModulationRoutingParameter(this, 293, 289, 246) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_BOTTOM;
			}
		});
	}
}
