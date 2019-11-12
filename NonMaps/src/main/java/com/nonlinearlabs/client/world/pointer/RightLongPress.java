package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;

class RightLongPress extends LongPress {

	RightLongPress(Gesture predecessor) {
		super(predecessor);
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
	public Control applyTo(Control receiver) {
		Control ret = receiver.longRightPress(getPosition());

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}
}
