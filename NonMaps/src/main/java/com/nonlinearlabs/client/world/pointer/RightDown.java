package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;

class RightDown extends Down {

	public RightDown() {
	}

	@Override
	public Gesture onRightUp() {
		return new RightUp(this);
	}

	@Override
	public Gesture move(boolean fine) {

		if (getDistance(getPosition(), PointerState.get().getPosition()) > getDragHysteresis())
			return new RightDragStart(this, getPosition(), PointerState.get().getPosition(), fine);

		return this;
	}

	@Override
	public Gesture onLongPress() {
		return new RightLongPress(this);
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.onContextMenu(getPosition());

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}
}
