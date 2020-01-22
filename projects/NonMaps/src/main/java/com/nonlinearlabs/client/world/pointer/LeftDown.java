package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;

class LeftDown extends Down {

	private boolean ctrl = false;

	public LeftDown(boolean ctrl) {
		this.ctrl = ctrl;
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
	public Gesture onLongPress() {
		return new LeftLongPress(this);
	}

	@Override
	public Control applyTo(Control receiver) {

		if (ctrl) {
			Control ret = receiver.onContextMenu(getPosition());

			if (ret != null) {
				setReceiver(receiver);
				return ret;
			}
		}
		return super.applyTo(receiver);
	}

}
