package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol4 extends AmountsCol {

	public AmountsCol4(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 258));
		addChild(new ModulationRoutingParameter(this, 263));
		addChild(new ModulationRoutingParameter(this, 268));
		addChild(new ModulationRoutingParameter(this, 273));
		addChild(new ModulationRoutingParameter(this, 278));
		addChild(new ModulationRoutingParameter(this, 283));
		addChild(new RibbonModulationRoutingParameter(this, 288));
		addChild(new RibbonModulationRoutingParameter(this, 293));
		addChild(new RibbonModulationRoutingParameter(this, 416));
		addChild(new RibbonModulationRoutingParameter(this, 422));
	}
}
