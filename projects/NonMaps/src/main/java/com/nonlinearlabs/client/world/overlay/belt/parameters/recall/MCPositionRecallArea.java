package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;

public class MCPositionRecallArea extends RecallArea {

	public MCPositionRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(value = new MCPositionRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		return EditBufferPresenterProvider.getPresenter().selectedParameter.modulation.isMCPosChanged;
	}

	@Override
	public void setVisibleForMode(BeltParameterLayout.Mode mode) {
		setVisible(mode == BeltParameterLayout.Mode.mcValue);
	}

	@Override
	public void resetValue() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		if (p.modulation.isMCPosChanged) {
			NonMaps.get().getServerProxy().recallMCPosForCurrentParameter(p.id);
			getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

}
