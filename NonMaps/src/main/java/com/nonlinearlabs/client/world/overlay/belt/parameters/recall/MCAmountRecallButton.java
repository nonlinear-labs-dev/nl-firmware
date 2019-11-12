package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

public class MCAmountRecallButton extends RecallButton {

	public MCAmountRecallButton(RecallArea parent) {
		super(parent);
	}

	@Override
	public Control click(Position p) {
		if (!isActive())
			return null;

		if (EditBufferModel.getSelectedParameter().isChanged()) {
			NonMaps.get().getServerProxy().recallMcAmountForCurrentParameter();
			getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return this;
		}
		return null;
	}
}
