package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.dataModel.setup.Setup;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.client.world.maps.parameters.ModulationRoutingParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterEditor;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.SourcesAndAmounts;
import com.nonlinearlabs.client.world.maps.parameters.value.QuantizedClippedValue;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.belt.parameters.RibbonContextMenu;

public abstract class Ribbon extends PhysicalControlParameter {

	private int currentMode = 0;

	Ribbon(HardwareSourcesCol1 parent) {
		super(parent);
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
	public boolean hasContextMenu() {
		return true;
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

	@Override
	protected boolean updateValues(Node child) {
		if (super.updateValues(child))
			return true;

		String nodeName = child.getNodeName();

		String value = ServerProxy.getText(child);

		if (nodeName.equals("ribbon-return-mode")) {
			int m = Integer.parseInt(value);
			if (m != currentMode) {
				currentMode = m;
				onRibbonModeChanged();
			}
			return true;
		}
		return getName().update(child);
	}

	private void onRibbonModeChanged() {
		ParameterEditor eddi = (ParameterEditor) getSelectionRoot();
		SourcesAndAmounts mappings = (SourcesAndAmounts) eddi.findParameterGroup("MCM");
		mappings.onReturningModeChanged(this);
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

	@Override
	public void addModulationRoutingParameter(final ModulationRoutingParameter modulationRoutingParameter) {
		super.addModulationRoutingParameter(modulationRoutingParameter);

		int target = modulationRoutingParameter.getTargetParameterID();
		Parameter mc = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().findParameter(target);
		mc.addListener(new ParameterListener() {

			@Override
			public void onParameterChanged(QuantizedClippedValue newValue) {
				if (getReturnMode() == ReturnMode.None
						&& modulationRoutingParameter.getValue().getQuantizedClipped() != 0.0) {
					getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, newValue.getQuantizedClipped());
				}
			}
		});
	}

}