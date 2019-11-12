package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;

public class MCPositionRecallArea extends RecallArea {

	public MCPositionRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new MCPositionRecallButton(this));
		addChild(value = new MCPositionRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		BasicParameterModel bpm = EditBufferModel.getSelectedParameter();
		if (bpm instanceof ModulateableParameterModel) {
			ModulateableParameterModel modP = (ModulateableParameterModel) bpm;
			return modP.isMCPosChanged();
		}
		return false;
	}

	@Override
	public void setVisibleForMode(BeltParameterLayout.Mode mode) {
		setVisible(mode == BeltParameterLayout.Mode.mcValue);
	}

}
