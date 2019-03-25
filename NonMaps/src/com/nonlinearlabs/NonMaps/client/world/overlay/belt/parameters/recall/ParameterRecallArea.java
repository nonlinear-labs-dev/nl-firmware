package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

public class ParameterRecallArea extends RecallArea {

	public ParameterRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new ParameterRecallButton(this));
		addChild(value = new ParameterRecallValue(this));
	}
	
	@Override
	public boolean isChanged() {
		return EditBufferModel.get().getSelectedParameter().isValueChanged();
	}

	@Override
	public void setVisibleForMode(Mode mode) {
		setVisible(mode == Mode.modulateableParameter || 
					mode == Mode.unmodulateableParameter ||
					mode == Mode.paramValue);
	}
	
}
