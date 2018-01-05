package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Position;

class LeftDrag extends Drag {

	LeftDrag(Gesture predecessor, Position to, boolean fine) {
		super(predecessor, to, fine);
	}

	@Override
	public Gesture onLeftUp() {
		return new LeftUp(this);
	}

	@Override
	public Gesture move(boolean fine) {
		return new LeftDrag(this, PointerState.get().getPosition(), fine);
	}

}
