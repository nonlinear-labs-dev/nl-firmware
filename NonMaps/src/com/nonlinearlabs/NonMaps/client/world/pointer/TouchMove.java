package com.nonlinearlabs.NonMaps.client.world.pointer;

import java.util.ArrayList;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.dom.client.Touch;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.pointer.Gesture.HasPosition;
import com.nonlinearlabs.NonMaps.client.world.pointer.Gesture.NeedsFocus;

class TouchMove extends Drag implements NeedsFocus, HasPosition {

	private ArrayList<Position> touches;

	TouchMove(Gesture predecessor, Position from, JsArray<Touch> touches) {
		super(predecessor, from, new Position(touches.get(0)), false);
		this.touches = new ArrayList<Position>();

		for (int i = 0; i < touches.length(); i++) {
			Touch t = touches.get(i);
			this.touches.add(new Position(t));
		}
	}

	@Override
	public Gesture onTouchStart(JsArray<Touch> touches) {
		return super.onTouchMove(touches);
	}

	@Override
	public Gesture onTouchMove(JsArray<Touch> touches) {
		if (touches.length() == 1) {
			return new TouchMove(this, getPosition(), touches);
		} else if (touches.length() == 2) {
			return new TouchPinchStart(this, touches);
		}

		return this;

	}

	@Override
	public Gesture onTouchEnd(JsArray<Touch> touches) {
		if (touches.length() > 0)
			return onTouchMove(touches);

		return new TouchMoveEnd(this);
	}

	@Override
	public Control applyTo(Control receiver) {
		HasPosition prev = (HasPosition) getPredecessor();
		Control ret = receiver.mouseDrag(prev.getPosition(), getPosition(), false);

		if (ret != null) {
			setReceiver(receiver);
		}

		return ret;
	}

	@Override
	public Position getPosition() {
		return touches.get(0);
	}

}
