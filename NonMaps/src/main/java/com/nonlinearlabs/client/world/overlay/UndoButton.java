package com.nonlinearlabs.client.world.overlay;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

class UndoButton extends UndoRedoButton {

	UndoButton(Control parent) {
		super(parent, "Undo_Disabled.svg", "Undo_Enabled.svg", "Undo_Enabled_pressed.svg");
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		getNonMaps().getServerProxy().undo();
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		return this;
	}

	public void update() {
		setEnabled(NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getUndoTree().getTree()
				.getCurrentUndoID() != 0);
	}

}
