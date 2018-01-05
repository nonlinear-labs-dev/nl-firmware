package com.nonlinearlabs.NonMaps.client.world.pointer;

import java.util.ArrayList;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.dom.client.Touch;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;

class TouchDown extends Down {

	private ArrayList<Position> touches;

	TouchDown(JsArray<Touch> touches) {
		this.touches = new ArrayList<Position>();

		for (int i = 0; i < touches.length(); i++) {
			Touch t = touches.get(i);
			this.touches.add(new Position(t));
		}
	}

	@Override
	public Gesture onTouchEnd(JsArray<Touch> touches) {
		if (touches.length() > 0)
			return onTouchMove(touches);

		return new TouchUp(this);
	}

	@Override
	public Gesture onTouchMove(JsArray<Touch> touches) {
		if (touches.length() == 1) {
			if (getDistance(this.touches.get(0), new Position(touches.get(0))) > Millimeter.toPixels(5)) {
				return new TouchMoveStart(this, touches);
			}

			return this;
		} else if (touches.length() == 2) {
			return new TouchPinchStart(this, touches);
		}
		return this;
	}

	@Override
	public Control applyTo(Control receiver) {
		if (touches.size() == 1)
			return super.applyTo(receiver);

		return null;
	}

	@Override
	public Gesture onLongPress() {
		return new TouchLongPress(this);
	}
}
