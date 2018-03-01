package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.dom.client.Touch;
import com.nonlinearlabs.NonMaps.client.world.Control;

class TouchLongPress extends LongPress {

	TouchLongPress(Gesture predecessor) {
		super(predecessor);
	}

	@Override
	public Gesture onTouchEnd(JsArray<Touch> touches) {
		if (touches.length() > 0)
			return onTouchMove(touches);

		return new TouchMoveEnd(this);
	}

	@Override
	public Gesture onTouchMove(JsArray<Touch> touches) {
		if (getDistance(getPosition(), PointerState.get().getPosition()) > getDragHysteresis())
			return new TouchMoveStart(this, getPosition(), touches);

		return this;
	}

	@Override
	public Control applyTo(Control receiver) {
		return null;
	}

}
