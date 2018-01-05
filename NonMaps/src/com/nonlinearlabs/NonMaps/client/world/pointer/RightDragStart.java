package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

class RightDragStart extends RightDrag {

	RightDragStart(Gesture predecessor, Position to, boolean fine) {
		super(predecessor, to, fine);
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.startDragging(getPosition());

		if (ret != null) {
			setReceiver(receiver);
			return super.applyTo(ret);
		}

		return null;
	}
}
