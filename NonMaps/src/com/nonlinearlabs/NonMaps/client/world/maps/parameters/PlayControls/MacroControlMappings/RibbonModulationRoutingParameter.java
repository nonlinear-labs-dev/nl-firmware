package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingButton;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingParameter;

public class RibbonModulationRoutingParameter extends ModulationRoutingParameter {

	public RibbonModulationRoutingParameter(MapsLayout parent, int paramID, int srcParamID, int targetParamID) {
		super(parent, paramID, srcParamID, targetParamID);
	}

	@Override
	protected ModulationRoutingButton createRoutingButton() {
		return new RibbonModulationRoutingButton(this);
	}

	@Override
	public boolean isOracle() {
		return false;
	}
}
