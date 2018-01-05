package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

class Wheel extends Terminus implements Gesture.HasPosition {

	private int delta;
	private boolean fine;
	private Position pos = PointerState.get().getPosition();

	public Wheel(int delta, boolean fine) {
		super(null);
		this.delta = delta;
		this.fine = fine;
	}

	@Override
	public Control applyTo(Control receiver) {
		return receiver.wheel(getPosition(), -delta, fine);
	}

	@Override
	public Position getPosition() {
		return pos;
	}
}
