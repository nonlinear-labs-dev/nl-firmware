package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol2 extends AmountsCol {

	public AmountsCol2(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 256));
		addChild(new ModulationRoutingParameter(this, 261));
		addChild(new ModulationRoutingParameter(this, 266));
		addChild(new ModulationRoutingParameter(this, 271));
		addChild(new ModulationRoutingParameter(this, 276));
		addChild(new ModulationRoutingParameter(this, 281));
		addChild(new RibbonModulationRoutingParameter(this, 286));
		addChild(new RibbonModulationRoutingParameter(this, 291));
		addChild(new RibbonModulationRoutingParameter(this, 414));
		addChild(new RibbonModulationRoutingParameter(this, 420));
	}
}
