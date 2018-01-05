package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.MouseUpEvent;
import com.google.gwt.event.dom.client.MouseUpHandler;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsMouseUpHandler implements MouseUpHandler {

	@Override
	public void onMouseUp(MouseUpEvent event) {
		event.preventDefault();

		Position p = new Position(event);

		if (event.getNativeButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_LEFT)
			PointerState.get().onLeftUp(p);
		else if (event.getNativeButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_RIGHT)
			PointerState.get().onRightUp(p);
	}
}