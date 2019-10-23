package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol2 extends AmountsCol {

	public AmountsCol2(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 256, 254, 244));
		addChild(new ModulationRoutingParameter(this, 261, 259, 244));
		addChild(new ModulationRoutingParameter(this, 266, 264, 244));
		addChild(new ModulationRoutingParameter(this, 271, 269, 244));
		addChild(new ModulationRoutingParameter(this, 276, 274, 244));
		addChild(new ModulationRoutingParameter(this, 281, 279, 244));
		addChild(new RibbonModulationRoutingParameter(this, 286, 284, 244));
		addChild(new RibbonModulationRoutingParameter(this, 291, 289, 244));
	}
}
