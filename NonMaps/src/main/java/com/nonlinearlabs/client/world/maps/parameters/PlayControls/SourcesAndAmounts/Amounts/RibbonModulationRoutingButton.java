package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingButton;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;

public class RibbonModulationRoutingButton extends ModulationRoutingButton {

	public RibbonModulationRoutingButton(ModulationRoutingParameter parent, int parameterID) {
		super(parent, parameterID,  "Control_Through_BiDi_Disabled.svg", "Control_Through_BiDi_Enabled.svg",
				"Control_Through_BiDi_Active.svg");
	}
}
