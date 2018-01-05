package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.dom.client.Touch;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.Tracer;
import com.nonlinearlabs.NonMaps.client.world.Control;

class TouchPinchStart extends TouchPinch {

	TouchPinchStart(Gesture predecessor, JsArray<Touch> touches) {
		super(predecessor, touches);
		NonMaps.theMaps.captureMouse();
	}

	@Override
	public Gesture onTouchMove(JsArray<Touch> touches) {
		return new TouchPinch(this, touches);
	}

	@Override
	public Gesture onTouchEnd(JsArray<Touch> touches) {
		if (touches.length() > 0)
			return onTouchMove(touches);

		return new TouchPinchEnd(this);
	}

	@Override
	public Control applyTo(Control receiver) {
		Tracer.log("TouchPinchStart.applyTo -- 1");
		Control ret = receiver.pinch(getPosition(), 0, this);

		if (ret != null)
			setReceiver(receiver);

		Tracer.log("TouchPinchStart.applyTo -- 2 - " + ret);
		return ret;
	}

}
