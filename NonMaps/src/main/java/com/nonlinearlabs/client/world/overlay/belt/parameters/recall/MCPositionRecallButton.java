package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

public class MCPositionRecallButton extends RecallButton {
	public MCPositionRecallButton(RecallArea parent) {
		super(parent);
	}

	@Override
	public Control click(Position pos) {
		BasicParameterModel p = EditBufferModel.get().getSelectedParameter();
		if (p instanceof ModulateableParameter) {
			if (((ModulateableParameter) p).isMCPosChanged()) {
				NonMaps.get().getServerProxy().recallMCPosForCurrentParameter();
				getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return this;
			}
		}
		return null;
	}
}
