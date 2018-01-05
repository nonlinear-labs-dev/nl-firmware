package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;

class UndoRedoButtons extends OverlayLayout {

	private UndoButton undo;
	private RedoButton redo;

	UndoRedoButtons(final Overlay overlay, Belt belt) {
		super(overlay);

		undo = addChild(new UndoButton(this));
		redo = addChild(new RedoButton(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		undo.doLayout(0, 0, w / 2, h);
		redo.doLayout(w / 2, 0, w / 2, h);
	}

	public void update() {
		undo.update();
		redo.update();
	}

	@Override
	public Control doubleClick() {
		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}
	
	@Override
	public Control mouseUp(Position eventPoint) {
		return this;
	}
	
	@Override
	public Control click(Position eventPoint) {
		return this;
	}

	@Override
	public Control longLeftPress(Position eventPoint) {
		return this;
	}
	
	@Override
	public Control longRightPress(Position eventPoint) {
		return this;
	}

	@Override
	public Control longPress(Position eventPoint) {
		return this;
	}
	
	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		return this;
	}
	
	@Override
	public Control onContextMenu(Position pos) {
		return this;
	}
}
