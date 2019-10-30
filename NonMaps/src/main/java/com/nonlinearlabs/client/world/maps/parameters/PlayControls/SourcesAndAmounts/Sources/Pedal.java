package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.belt.parameters.PedalContextMenu;

public class Pedal extends PhysicalControlParameter {

	private int currentMode = 0;

	Pedal(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	public PedalContextMenu createContextMenu(Overlay o) {
		return new PedalContextMenu(o, this);
	}

	@Override
	public ReturnMode getReturnMode() {
		switch (currentMode) {
		case 0:
			return ReturnMode.None;

		case 1:
			return ReturnMode.Zero;

		case 2:
			return ReturnMode.Center;
		}
		return ReturnMode.None;
	}

}