package com.nonlinearlabs.client.world.overlay;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

class RedoButton extends UndoRedoButton {

	RedoButton(Control parent) {
		super(parent, "Redo_Disabled.svg", "Redo_Enabled.svg", "Redo_Enabled_pressed.svg");
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		getNonMaps().getServerProxy().redo();
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		return this;
	}

	public void update() {
		setEnabled(NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getUndoTree().getTree()
				.getCurrentRedoID() != 0);
	}
}
