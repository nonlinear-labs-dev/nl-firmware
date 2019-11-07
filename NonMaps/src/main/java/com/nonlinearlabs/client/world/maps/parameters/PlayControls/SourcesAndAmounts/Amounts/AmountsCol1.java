package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol1 extends AmountsCol {

	public AmountsCol1(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 255));
		addChild(new ModulationRoutingParameter(this, 260));
		addChild(new ModulationRoutingParameter(this, 265));
		addChild(new ModulationRoutingParameter(this, 270));
		addChild(new ModulationRoutingParameter(this, 275));
		addChild(new ModulationRoutingParameter(this, 280));
		addChild(new RibbonModulationRoutingParameter(this, 285));
		addChild(new RibbonModulationRoutingParameter(this, 290));
	}
}
