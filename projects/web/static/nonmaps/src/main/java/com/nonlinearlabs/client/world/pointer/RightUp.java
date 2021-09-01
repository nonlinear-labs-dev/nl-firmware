package com.nonlinearlabs.client.world.pointer;

class RightUp extends Up {

	RightUp(Gesture predecessor) {
		super(predecessor);
	}

	public Gesture getFollower() {
		if (wasClick())
			return new RightClick(this);

		return null;
	}
}
