package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings;

import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingButton;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingParameter;

public class RibbonModulationRoutingButton extends ModulationRoutingButton {

	public RibbonModulationRoutingButton(ModulationRoutingParameter parent) {
		super(parent, "Control_Through_BiDi_Disabled.svg", "Control_Through_BiDi_Enabled.svg",
				"Control_Through_BiDi_Active.svg");
	}	
}
