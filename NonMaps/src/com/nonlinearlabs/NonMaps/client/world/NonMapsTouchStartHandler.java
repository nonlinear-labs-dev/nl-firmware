package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.TouchStartEvent;
import com.google.gwt.event.dom.client.TouchStartHandler;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsTouchStartHandler implements TouchStartHandler {

	@Override
	public void onTouchStart(TouchStartEvent event) {
		event.preventDefault();
		event.stopPropagation();

		PointerState.get().onTouchStart(event.getTouches());
		NonMaps.theMaps.captureFocus();
	}
}