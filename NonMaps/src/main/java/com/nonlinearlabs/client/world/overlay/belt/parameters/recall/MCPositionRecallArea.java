package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;

public class MCPositionRecallArea extends RecallArea {

	public MCPositionRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(button = new MCPositionRecallButton(this));
		addChild(value = new MCPositionRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.modulation.isMCPosChanged;
	}

	@Override
	public void setVisibleForMode(BeltParameterLayout.Mode mode) {
		setVisible(mode == BeltParameterLayout.Mode.mcValue);
	}

}
