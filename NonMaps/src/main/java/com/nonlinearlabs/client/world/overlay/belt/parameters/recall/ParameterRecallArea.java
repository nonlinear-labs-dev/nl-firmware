package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

public class ParameterRecallArea extends RecallArea {

	public ParameterRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new ParameterRecallButton(this));
		addChild(value = new ParameterRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.valueChanged;
	}

	@Override
	public void setVisibleForMode(Mode mode) {
		setVisible(
				mode == Mode.modulateableParameter || mode == Mode.unmodulateableParameter || mode == Mode.paramValue);
	}

}
