package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol5 extends AmountsCol {

	public AmountsCol5(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 373));
		addChild(new ModulationRoutingParameter(this, 375));
		addChild(new ModulationRoutingParameter(this, 377));
		addChild(new ModulationRoutingParameter(this, 379));
		addChild(new ModulationRoutingParameter(this, 381));
		addChild(new ModulationRoutingParameter(this, 383));
		addChild(new RibbonModulationRoutingParameter(this, 385));
		addChild(new RibbonModulationRoutingParameter(this, 387));
		addChild(new RibbonModulationRoutingParameter(this, 417));
		addChild(new RibbonModulationRoutingParameter(this, 423));
	}
}
