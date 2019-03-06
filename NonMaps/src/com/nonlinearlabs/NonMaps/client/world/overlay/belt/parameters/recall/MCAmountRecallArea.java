package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout;

public class MCAmountRecallArea extends RecallArea {
		
	public MCAmountRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new MCAmountRecallButton(this));
		addChild(value = new MCAmountRecallValue(this));
	}
		
	@Override
	public boolean isChanged() {		
		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel bpm = eb.findParameter(eb.selectedParameter.getValue());
		if(bpm instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter)bpm;
			return modP.ogModAmount.getValue() != modP.modAmount.getValue();
		}
		return false;
	}

}
