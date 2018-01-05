package com.nonlinearlabs.NonMaps.client.world.pointer;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.pointer.Gesture.HasPosition;
import com.nonlinearlabs.NonMaps.client.world.pointer.Gesture.NeedsFocus;

class TouchMoveEnd extends TouchEnd implements HasPosition, NeedsFocus {

	private Position pos = PointerState.get().getPosition();

	TouchMoveEnd(Gesture predecessor) {
		super(predecessor);
		NonMaps.theMaps.releaseMouse();
	}

	@Override
	public Control applyTo(Control receiver) {
		return null;
	}

	public Gesture getFollower() {
		return new TouchUp(this);
	}

	@Override
	public Position getPosition() {
		return pos;
	}
}
