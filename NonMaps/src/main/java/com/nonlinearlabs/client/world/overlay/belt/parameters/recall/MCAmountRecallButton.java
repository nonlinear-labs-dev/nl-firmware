package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

public class MCAmountRecallButton extends RecallButton {

	public MCAmountRecallButton(RecallArea parent) {
		super(parent);
	}

	@Override
	public Control click(Position position) {
		if (!isActive())
			return null;

		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;

		if (p.modulation.isModAmountChanged) {
			NonMaps.get().getServerProxy().recallMcAmountForCurrentParameter();
			getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return this;
		}
		return null;
	}
}
