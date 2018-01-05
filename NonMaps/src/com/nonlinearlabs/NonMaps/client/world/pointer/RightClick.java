package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Control;

class RightClick extends Click {

	RightClick(Gesture predecessor) {
		super(predecessor);
	}

	@Override
	public Control applyTo(Control receiver) {
		return null;
	}

}
