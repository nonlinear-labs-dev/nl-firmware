package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls;

import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SpacerLarge;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.Pedal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings.MacroControlMappings;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros;

class MappingAndMacros extends LayoutResizingVertical {

	private Macros macros;
	private MacroControlMappings mcm;

	MappingAndMacros(PlayControls parent) {
		super(parent);

		addChild(mcm = new MacroControlMappings(this));
		addChild(new SpacerLarge(parent));
		macros = addChild(new Macros(this));
	}

	public Macros getMacros() {
		return macros;
	}

	@Override
	public Direction getLayoutDirection() {
		return Direction.RIGHT;
	}

	public Pedal getPedal(int i) {
		return mcm.getPedal(i);
	}
}
