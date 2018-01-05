package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.MouseWheelEvent;
import com.google.gwt.event.dom.client.MouseWheelHandler;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsWheelHandler implements MouseWheelHandler {

	@Override
	public void onMouseWheel(final MouseWheelEvent event) {
		Position pos = new Position(event);
		PointerState.get().onWheel(pos, event.getDeltaY(), event.isControlKeyDown());
		event.preventDefault();
	}
}