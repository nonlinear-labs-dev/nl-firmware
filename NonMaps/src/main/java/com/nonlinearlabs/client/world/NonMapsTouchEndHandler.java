package com.nonlinearlabs.client.world;

import com.google.gwt.event.dom.client.TouchEndEvent;
import com.google.gwt.event.dom.client.TouchEndHandler;
import com.nonlinearlabs.client.world.pointer.PointerState;

class NonMapsTouchEndHandler implements TouchEndHandler {

	@Override
	public void onTouchEnd(TouchEndEvent event) {
		event.preventDefault();
		PointerState.get().onTouchEnd(event.getTouches());
	}
}