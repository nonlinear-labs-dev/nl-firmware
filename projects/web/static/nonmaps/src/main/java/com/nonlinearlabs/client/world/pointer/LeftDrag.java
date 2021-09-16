package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Position;

class LeftDrag extends Drag {

	LeftDrag(Gesture predecessor, Position from, Position to, boolean fine) {
		super(predecessor, from, to, fine);
	}

	@Override
	public Gesture onLeftUp() {
		return new LeftUp(this);
	}

	@Override
	public Gesture move(boolean fine) {
		return new LeftDrag(this, getPosition(), PointerState.get().getPosition(), fine);
	}

}
