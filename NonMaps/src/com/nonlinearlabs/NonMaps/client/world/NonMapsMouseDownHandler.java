package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.MouseDownEvent;
import com.google.gwt.event.dom.client.MouseDownHandler;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsMouseDownHandler implements MouseDownHandler {

	@Override
	public void onMouseDown(MouseDownEvent event) {
		event.preventDefault();
		event.stopPropagation();

		Position p = new Position(event);

		if (event.getNativeButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_LEFT)
			PointerState.get().onLeftDown(p, event.isControlKeyDown());
		else if (event.getNativeButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_RIGHT)
			PointerState.get().onRightDown(p);

		NonMaps.theMaps.captureFocus();
	}

}