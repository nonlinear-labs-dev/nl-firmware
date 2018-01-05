package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.world.Control;

class Terminus extends Gesture {

	Terminus(Gesture predecessor) {
		super(predecessor);
	}

	@Override
	public Control applyTo(Control receiver) {
		return null;
	}

	public boolean isTerminus() {
		return true;
	}
}
