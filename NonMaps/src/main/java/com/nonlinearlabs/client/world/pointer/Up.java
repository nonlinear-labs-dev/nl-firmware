package com.nonlinearlabs.client.world.pointer;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

class Up extends Gesture implements Gesture.NeedsFocus, Gesture.HasPosition {

	private Position pos;

	public Up(Gesture predecessor) {
		super(predecessor);
		NonMaps.theMaps.releaseMouse();
		this.pos = PointerState.get().getPosition().copy();
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.mouseUp(getPosition());

		if (ret != null)
			setReceiver(receiver);

		return ret;
	}

	@Override
	public Position getPosition() {
		return pos;
	}

	protected boolean wasClick() {
		Gesture down = recursePredecessors(new GestureInspector() {

			@Override
			public boolean onGestureFound(Gesture g) {
				return g instanceof Down;
			}
		});

		Gesture longPress = recursePredecessors(new GestureInspector() {

			@Override
			public boolean onGestureFound(Gesture g) {
				return g instanceof LongPress;
			}
		});

		Gesture handledMove = recursePredecessors(new GestureInspector() {

			@Override
			public boolean onGestureFound(Gesture g) {
				if (g instanceof Drag || g instanceof TouchMove)
					return g.getReceiver() != null;

				return false;
			}
		});

		boolean sameReceiver = (down.getReceiver() == getReceiver()) || (getReceiver() == null);
		boolean wasLongPressHandled = (longPress != null) && (longPress.getReceiver() != null);
		boolean moveHandled = handledMove != null;
		boolean hit = false;

		if (down.getReceiver() != null) {
			hit = down.getReceiver().getPixRect().contains(getPosition());
		}

		boolean ret = !wasLongPressHandled && !moveHandled && sameReceiver && hit;

		return ret;
	}

	public boolean isTerminus() {
		return !wasClick();
	}

}