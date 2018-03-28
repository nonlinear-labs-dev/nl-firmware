package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls;

import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SpacerLarge;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.SourcesAndAmounts;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;

class SourcesAmountsAndMacros extends LayoutResizingVertical {

	private Macros macros;
	private SourcesAndAmounts mcm;

	SourcesAmountsAndMacros(PlayControls parent) {
		super(parent);

		addChild(mcm = new SourcesAndAmounts(this));
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
