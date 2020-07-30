package com.nonlinearlabs.client.world.pointer;

class LeftUp extends Up {

	public LeftUp(Gesture predecessor) {
		super(predecessor);
	}

	public Gesture getFollower() {
		if (wasClick())
			return new LeftClick(this);

		return null;
	}

}
