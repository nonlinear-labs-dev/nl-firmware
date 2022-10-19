package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol3 extends AmountsCol {

	public AmountsCol3(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 257));
		addChild(new ModulationRoutingParameter(this, 262));
		addChild(new ModulationRoutingParameter(this, 267));
		addChild(new ModulationRoutingParameter(this, 272));
		addChild(new ModulationRoutingParameter(this, 277));
		addChild(new ModulationRoutingParameter(this, 282));
		addChild(new RibbonModulationRoutingParameter(this, 287));
		addChild(new RibbonModulationRoutingParameter(this, 292));
		addChild(new RibbonModulationRoutingParameter(this, 415));
		addChild(new RibbonModulationRoutingParameter(this, 421));
	}
}
