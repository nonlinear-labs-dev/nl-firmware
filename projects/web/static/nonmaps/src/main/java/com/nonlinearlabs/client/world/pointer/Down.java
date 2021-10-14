package com.nonlinearlabs.client.world.pointer;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;

abstract public class Down extends Gesture implements Gesture.HasPosition {

	private Position pos;

	public Down() {
		super(null);

		NonMaps.theMaps.captureMouse();

		this.pos = PointerState.get().getPosition().copy();

		Scheduler.get().scheduleFixedDelay(new RepeatingCommand() {

			@Override
			public boolean execute() {
				PointerState.get().longPressTimerExpired(Down.this);
				return false;
			}
		}, 500);
	}

	@Override
	public Position getPosition() {
		return pos;
	}

	@Override
	public Control applyTo(Control receiver) {
		Control ret = receiver.mouseDown(getPosition());

		if (ret != null) {
			setReceiver(receiver);
		}

		return ret;
	}

	public abstract Gesture onLongPress();
}