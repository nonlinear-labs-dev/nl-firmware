package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

public class ParameterRecallArea extends RecallArea {

	public ParameterRecallArea(BeltParameterLayout parent) {
		super(parent);
		addChild(value = new ParameterRecallValue(this));
	}

	@Override
	public boolean isChanged() {
		return EditBufferPresenterProvider.getPresenter().selectedParameter.valueChanged;
	}

	@Override
	public void setVisibleForMode(Mode mode) {
		setVisible(mode.isOneOf(Mode.modulateableParameter, Mode.unmodulateableParameter, Mode.paramValue));
	}

	@Override
	public void resetValue() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		if (p.changed) {
			NonMaps.get().getServerProxy().recallCurrentParameterFromPreset(p.id);
			getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

}
