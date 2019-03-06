package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout;

public class ParameterRecallArea extends RecallArea {

	public ParameterRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new ParameterRecallButton(this));
		addChild(value = new ParameterRecallValue(this));
	}
	
	@Override
	public boolean isChanged() {
		EditBufferModel eb = EditBufferModel.get();
		BasicParameterModel bpm = eb.getSelectedParameter();
		return bpm.value.value.getValue() != bpm.originalValue.getValue();
	}

}
