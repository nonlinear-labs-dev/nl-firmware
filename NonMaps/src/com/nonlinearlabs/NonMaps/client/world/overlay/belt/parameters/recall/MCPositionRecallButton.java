package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

public class MCPositionRecallButton extends RecallButton {
	public MCPositionRecallButton(RecallArea parent) {
		super(parent);
	}

	@Override
	public Control click(Position pos) {
		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel p = eb.findParameter(eb.selectedParameter.getValue());
		if(p instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter)p;
			if(modP.isMCPosChanged()) {
				NonMaps.get().getServerProxy().recallMCPosForCurrentParameter();
				getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return this;
			}

		}
		return null;
	}
}
