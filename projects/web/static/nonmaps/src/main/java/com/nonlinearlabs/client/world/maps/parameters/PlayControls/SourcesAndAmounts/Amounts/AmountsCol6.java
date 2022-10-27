package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol6 extends AmountsCol {

	public AmountsCol6(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 374) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_TOP;
			}
		});
		addChild(new ModulationRoutingParameter(this, 376));
		addChild(new ModulationRoutingParameter(this, 378));
		addChild(new ModulationRoutingParameter(this, 380));
		addChild(new ModulationRoutingParameter(this, 382));
		addChild(new ModulationRoutingParameter(this, 384));
		addChild(new RibbonModulationRoutingParameter(this, 386));
		addChild(new RibbonModulationRoutingParameter(this, 388));
		addChild(new RibbonModulationRoutingParameter(this, 418));
		addChild(new RibbonModulationRoutingParameter(this, 424) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_BOTTOM;
			}
		});
	}
}
