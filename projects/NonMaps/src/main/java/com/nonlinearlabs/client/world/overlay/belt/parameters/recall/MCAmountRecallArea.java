package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

public class MCAmountRecallArea extends RecallArea {

	public MCAmountRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(value = new MCAmountRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		return EditBufferPresenterProvider.getPresenter().selectedParameter.modulation.isModAmountChanged;
	}

	@Override
	public void setVisibleForMode(Mode mode) {
		setVisible(mode == Mode.mcAmount);
	}

	@Override
	public void resetValue() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;

		if (p.modulation.isModAmountChanged) {
			NonMaps.get().getServerProxy().recallMcAmountForCurrentParameter(p.id);
			getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}
}
