package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout;

public class MCPositionRecallArea extends RecallArea {
	
	public MCPositionRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new MCPositionRecallButton(this));
		addChild(value = new MCPositionRecallValue(this));
	}
	
	@Override
	public boolean isChanged() {
		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel bpm = eb.findParameter(eb.selectedParameter.getValue());
		if(bpm instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter)bpm;
			return modP.isMCPosChanged();
		}
		return false;
	}
}

