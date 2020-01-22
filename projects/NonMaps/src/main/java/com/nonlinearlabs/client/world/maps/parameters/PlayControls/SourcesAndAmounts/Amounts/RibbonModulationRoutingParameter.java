package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingButton;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class RibbonModulationRoutingParameter extends ModulationRoutingParameter {

	public RibbonModulationRoutingParameter(MapsLayout parent, int paramID) {
		super(parent, paramID);
	}

	@Override
	protected ModulationRoutingButton createRoutingButton() {
		return new RibbonModulationRoutingButton(this, getParameterNumber());
	}

	@Override
	public boolean isOracle() {
		return false;
	}
}
