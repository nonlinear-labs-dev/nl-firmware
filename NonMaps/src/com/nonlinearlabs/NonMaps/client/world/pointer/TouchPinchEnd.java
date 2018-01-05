package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.NonMaps;

class TouchPinchEnd extends Terminus {

	TouchPinchEnd(Gesture predecessor) {
		super(predecessor);
		NonMaps.theMaps.releaseMouse();
	}

}
