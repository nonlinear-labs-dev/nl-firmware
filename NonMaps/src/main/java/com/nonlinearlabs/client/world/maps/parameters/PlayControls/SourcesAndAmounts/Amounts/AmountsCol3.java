package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol3 extends AmountsCol {

	public AmountsCol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 257, 254, 245));
		addChild(new ModulationRoutingParameter(this, 262, 259, 245));
		addChild(new ModulationRoutingParameter(this, 267, 264, 245));
		addChild(new ModulationRoutingParameter(this, 272, 269, 245));
		addChild(new ModulationRoutingParameter(this, 277, 274, 245));
		addChild(new ModulationRoutingParameter(this, 282, 279, 245));
		addChild(new RibbonModulationRoutingParameter(this, 287, 284, 245));
		addChild(new RibbonModulationRoutingParameter(this, 292, 289, 245));
	}
}
