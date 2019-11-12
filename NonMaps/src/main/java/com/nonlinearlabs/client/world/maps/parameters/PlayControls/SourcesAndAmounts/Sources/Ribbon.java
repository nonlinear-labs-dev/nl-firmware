package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.belt.parameters.RibbonContextMenu;

public class Ribbon extends PhysicalControlParameter {

	Ribbon(HardwareSourcesCol1 parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	public RibbonContextMenu createContextMenu(Overlay o) {
		boolean showContextMenus = SetupModel.get().localSettings.contextMenus.getValue() == BooleanValues.on;

		if (showContextMenus) {
			return new RibbonContextMenu(o, this);
		}
		return null;
	}
}