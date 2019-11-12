package com.nonlinearlabs.client.world.pointer;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.dom.client.Touch;
import com.nonlinearlabs.client.world.Control;

final class Identity extends Gesture {

	public Identity() {
		super(null);
	}

	@Override
	public Gesture onLeftDown(boolean ctrl) {
		return new LeftDown(ctrl);
	}

	@Override
	public Gesture onLeftDoubleClick() {
		return new LeftDoubleClick();
	}

	@Override
	public Gesture onRightDown() {
		return new RightDown();
	}

	@Override
	public Gesture onRightDoubleClick() {
		return new RightDoubleClick();
	}

	public Gesture onTouchStart(JsArray<Touch> touches) {
		return new TouchDown(touches);
	}

	@Override
	public Gesture move(boolean fine) {
		return this;
	}

	@Override
	public Control applyTo(Control receiver) {
		return null;
	}
}
