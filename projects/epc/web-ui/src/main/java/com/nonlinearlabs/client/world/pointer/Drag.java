package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

class Drag extends Gesture implements Gesture.NeedsFocus, Gesture.HasPosition {

	private Position to;
	private Position from;
	private boolean fine = false;

	public Drag(Gesture predecessor, Position from, Position to, boolean fine) {
		super(predecessor);
		this.from = from;
		this.to = to.copy();
		this.fine = fine;
	}

	@Override
	public Position getPosition() {
		return to;
	}

	public Position getStartPosition() {
		return from;
	}

	public Position getEndPosition() {
		return to;
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.mouseDrag(from(), to(), fine);

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}

	private Position from() {
		Gesture g = recursePredecessors(new GestureInspector() {

			@Override
			public boolean onGestureFound(Gesture g) {
				return g instanceof HasPosition;
			}
		});

		if (g != null) {
			Gesture.HasPosition h = (Gesture.HasPosition) g;
			return h.getPosition();
		}

		return to();
	}

	private Position to() {
		return getPosition();
	}

}