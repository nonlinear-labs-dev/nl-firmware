package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;

class RightClick extends Click {

	RightClick(Gesture predecessor) {
		super(predecessor);
	}

	@Override
	public Control applyTo(Control receiver) {
		return null;
	}

}
