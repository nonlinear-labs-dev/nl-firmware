package com.nonlinearlabs.client.world.overlay.undo;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.client.world.overlay.Overlay;

public class UndoTransactionContextMenu extends ContextMenu {

	public UndoTransactionContextMenu(Overlay parent, final UndoTransaction undoTransaction) {
		super(parent);

		if (!undoTransaction.isParentOfCurrentUndo()) {
			addChild(new ContextMenuItem(this, "Delete from here on") {
				@Override
				public Control click(Position eventPoint) {
					undoTransaction.erase();
					return super.click(eventPoint);
				}
			});
		} else {
			addChild(new ContextMenuItem(this, "Make this the new root") {
				@Override
				public Control click(Position eventPoint) {
					undoTransaction.rebase();
					return super.click(eventPoint);
				}
			});
		}
	}
}
