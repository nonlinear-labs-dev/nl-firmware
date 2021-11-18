package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.belt.parameters.PedalContextMenu;

public class Pedal extends PhysicalControlParameter {

	Pedal(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	public PedalContextMenu createContextMenu(Overlay o) {
		return new PedalContextMenu(o, this);
	}

}