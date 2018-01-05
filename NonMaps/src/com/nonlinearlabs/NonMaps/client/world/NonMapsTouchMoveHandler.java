package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.TouchMoveEvent;
import com.google.gwt.event.dom.client.TouchMoveHandler;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsTouchMoveHandler implements TouchMoveHandler {

	@Override
	public void onTouchMove(TouchMoveEvent event) {
		event.preventDefault();

		PointerState.get().onTouchMove(event.getTouches());
	}
}