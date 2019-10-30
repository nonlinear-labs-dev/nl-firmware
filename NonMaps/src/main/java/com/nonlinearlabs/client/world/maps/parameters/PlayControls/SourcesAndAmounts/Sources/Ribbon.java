package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.dataModel.setup.Setup;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.belt.parameters.RibbonContextMenu;

public class Ribbon extends PhysicalControlParameter {

	private int currentMode = 0;

	Ribbon(HardwareSourcesCol1 parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	public RibbonContextMenu createContextMenu(Overlay o) {
		boolean showContextMenus = Setup.get().localSettings.contextMenus.getValue() == BooleanValues.on;

		if (showContextMenus) {
			return new RibbonContextMenu(o, this);
		}
		return null;
	}

	@Override
	public ReturnMode getReturnMode() {
		switch (currentMode) {
		case 0:
			return ReturnMode.None;

		case 1:
			return ReturnMode.Center;
		}
		return ReturnMode.None;
	}

}