package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

class LeftDragStart extends LeftDrag {

	LeftDragStart(Gesture predecessor, Position from, Position to, boolean fine) {
		super(predecessor, from, to, fine);
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.startDragging(getStartPosition());

		if (ret != null) {
			setReceiver(receiver);
			return super.applyTo(ret);
		}

		return null;
	}
}
