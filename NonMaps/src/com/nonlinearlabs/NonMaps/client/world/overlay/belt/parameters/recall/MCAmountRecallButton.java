package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

public class MCAmountRecallButton extends RecallButton {
	
	public MCAmountRecallButton(RecallArea parent) {
		super(parent);
	}

	@Override
	public Control click(Position p) {
		if(!isActive())
			return null;
		
		EditBufferModel eb = EditBufferModel.get();

		if(eb.findParameter(eb.selectedParameter.getValue()).isChanged()) {
			NonMaps.get().getServerProxy().recallMcAmountForCurrentParameter();
			getParent().getParent().invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return this;
		}
		return null;
	}
}
