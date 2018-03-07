package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.dom.client.Touch;
import com.google.gwt.user.client.Timer;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

class TouchMoveStart extends TouchMove {

	private Timer longPressTimer;

	TouchMoveStart(Gesture predecessor, Position from, JsArray<Touch> touches) {
		super(predecessor, from, touches);

		NonMaps.theMaps.captureMouse();

		longPressTimer = new Timer() {
			public void run() {
				PointerState.get().longPressTimerExpired(TouchMoveStart.this);
			}
		};
		longPressTimer.schedule(500);
	}

	@Override
	public Gesture onTouchMove(JsArray<Touch> touches) {
		return new TouchMove(this, getPosition(), touches);
	}

	public Gesture onLongPress() {
		return new TouchLongPress(this);
	}

	@Override
	public Gesture onTouchEnd(JsArray<Touch> touches) {
		if (touches.length() > 0)
			return onTouchMove(touches);

		return new TouchMoveEnd(this);
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.startDragging(getStartPosition());

		if (ret != null) {
			setReceiver(receiver);
			return super.applyTo(ret);
		}

		return ret;
	}
}
