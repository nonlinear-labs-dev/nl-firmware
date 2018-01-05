package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.MouseMoveEvent;
import com.google.gwt.event.dom.client.MouseMoveHandler;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsMouseMoveHandler implements MouseMoveHandler {

	@Override
	public void onMouseMove(MouseMoveEvent event) {
		event.preventDefault();
		event.stopPropagation();

		Position p = new Position(event);
		PointerState.get().onMove(p, event.isShiftKeyDown());
	}
}