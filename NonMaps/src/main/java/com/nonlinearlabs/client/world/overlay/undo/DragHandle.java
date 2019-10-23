package com.nonlinearlabs.client.world.overlay.undo;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class DragHandle extends SVGImage {

	double yDiffOnMouseDown = 0;
	double xDiffOnMouseDown = 0;

	public DragHandle(UndoTree parent) {
		super(parent, "UndoTree_Drag_Edge_B.svg");
	}

	@Override
	public UndoTree getParent() {
		return (UndoTree) super.getParent();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		double halfPi = Math.PI / 4;
		Position center = getPixRect().getCenterPoint();
		double yrot = center.getY() + Math.sin(halfPi) * (eventPoint.getX() - center.getX())
				+ Math.cos(halfPi) * (eventPoint.getY() - center.getY());

		if (yrot <= center.getY())
			return null;

		yDiffOnMouseDown = eventPoint.getY() - getPixRect().getTop();
		xDiffOnMouseDown = eventPoint.getX() - getPixRect().getLeft();
		return this;
	}

	@Override
	public Control startDragging(Position pos) {
		return this;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		getParent().onDragHandleMoved(newPoint.getX() - oldPoint.getX(), newPoint.getY() - oldPoint.getY());
		return this;
	}

}
