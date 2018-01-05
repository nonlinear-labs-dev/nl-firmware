package com.nonlinearlabs.NonMaps.client.world.pointer;


class TouchUp extends Up {

	TouchUp(Gesture predecessor) {
		super(predecessor);
	}

	public Gesture getFollower() {
		if (wasClick())
			return new TouchClick(this);

		return null;
	}

}
