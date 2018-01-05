package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

class DoubleClick extends Terminus implements Gesture.HasPosition {

	private Position pos;

	public DoubleClick() {
		super(null);

		this.pos = PointerState.get().getPosition();
	}

	@Override
	public Position getPosition() {
		return pos;
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.doubleClick();

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}

}