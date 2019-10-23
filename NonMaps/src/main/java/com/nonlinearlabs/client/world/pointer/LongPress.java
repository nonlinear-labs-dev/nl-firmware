package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Position;

abstract class LongPress extends Gesture implements Gesture.NeedsFocus, Gesture.HasPosition {
	private Position pos;

	LongPress(Gesture predecessor) {
		super(predecessor);
		this.pos = PointerState.get().getPosition().copy();
	}

	@Override
	public Position getPosition() {
		return pos;
	}
}
