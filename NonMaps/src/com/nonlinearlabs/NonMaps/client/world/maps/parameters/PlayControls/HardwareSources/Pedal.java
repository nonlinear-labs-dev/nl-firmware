package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterEditor;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings.MacroControlMappings;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.PedalContextMenu;

public abstract class Pedal extends PhysicalControlParameter {

	private int currentMode = 0;

	Pedal(MapsLayout parent) {
		super(parent);
	}

	@Override
	public PedalContextMenu createContextMenu(Overlay o) {
		return new PedalContextMenu(o, this);
	}

	@Override
	public boolean hasContextMenu() {
		return true;
	}

	@Override
	protected boolean updateValues(Node child) {
		if (super.updateValues(child))
			return true;

		String nodeName = child.getNodeName();

		String value = ServerProxy.getText(child);

		if (nodeName.equals("pedal-mode")) {
			int m = Integer.parseInt(value);
			if (m != currentMode) {
				currentMode = m;
				onPedalModeChanged();
			}
			return true;
		}
		return getName().update(child);
	}

	private void onPedalModeChanged() {
		ParameterEditor eddi = (ParameterEditor) getSelectionRoot();
		MacroControlMappings mappings = (MacroControlMappings) eddi.findParameterGroup("MCM");
		mappings.onReturningModeChanged(this);
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
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