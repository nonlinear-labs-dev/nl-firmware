package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;

class LeftLongPress extends LongPress {

	LeftLongPress(Gesture predecessor) {
		super(predecessor);
	}

	@Override
	public Gesture onLeftUp() {
		return new LeftUp(this);
	}

	@Override
	public Gesture move(boolean fine) {
		if (getDistance(getPosition(), PointerState.get().getPosition()) > getDragHysteresis())
			return new LeftDragStart(this, getPosition(), PointerState.get().getPosition(), fine);

		return this;
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.longLeftPress(getPosition());

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}

}
