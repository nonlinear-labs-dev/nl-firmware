package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.NonMaps;

class TouchPinchEnd extends Terminus {

	TouchPinchEnd(Gesture predecessor) {
		super(predecessor);
		NonMaps.theMaps.releaseMouse();
	}

}
