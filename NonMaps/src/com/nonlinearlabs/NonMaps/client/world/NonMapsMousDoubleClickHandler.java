package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.event.dom.client.DoubleClickEvent;
import com.google.gwt.event.dom.client.DoubleClickHandler;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

class NonMapsMousDoubleClickHandler implements DoubleClickHandler {

	@Override
	public void onDoubleClick(DoubleClickEvent event) {
		event.preventDefault();
		event.stopPropagation();

		Position p = new Position(event);

		if (event.getNativeButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_LEFT)
			PointerState.get().onLeftDoubleClick(p);
		else if (event.getNativeButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_RIGHT)
			PointerState.get().onRightDoubleClick(p);
	}
}
