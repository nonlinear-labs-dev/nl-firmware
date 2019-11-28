package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

public class MCAmountRecallArea extends RecallArea {

	public MCAmountRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new MCAmountRecallButton(this));
		addChild(value = new MCAmountRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.modulation.isModAmountChanged;
	}

	@Override
	public void setVisibleForMode(Mode mode) {
		setVisible(mode == Mode.mcAmount);
	}
}
