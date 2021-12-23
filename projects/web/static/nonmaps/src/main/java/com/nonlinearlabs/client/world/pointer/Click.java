package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

class Click extends Terminus implements Gesture.HasPosition {

	public Click(Gesture predecessor) {
		super(predecessor);
	}

	@Override
	public Position getPosition() {
		Gesture p = recursePredecessors(new GestureInspector() {

			@Override
			public boolean onGestureFound(Gesture g) {
				return g instanceof HasPosition;
			}
		});

		return ((HasPosition) p).getPosition();
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.click(getPosition());

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}
}