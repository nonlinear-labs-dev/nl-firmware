package com.nonlinearlabs.client.world.pointer;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.google.gwt.core.client.Scheduler.ScheduledCommand;
import com.nonlinearlabs.client.Tracer;
import com.nonlinearlabs.client.world.Control;

class TouchClick extends Click {

	static private boolean isWaitingForDoubleClick = false;

	TouchClick(Gesture predecessor) {
		super(predecessor);

		if (!isWaitingForDoubleClick) {
			isWaitingForDoubleClick = true;

			Scheduler.get().scheduleFixedDelay(new RepeatingCommand() {

				@Override
				public boolean execute() {
					isWaitingForDoubleClick = false;
					return false;
				}
			}, 250);
		} else {
			isWaitingForDoubleClick = false;

			Scheduler.get().scheduleFinally(new ScheduledCommand() {

				@Override
				public void execute() {
					PointerState.get().onLeftDoubleClick(getPosition());
				}
			});
		}
	}

	@Override
	public Control applyTo(Control receiver) {

		Gesture longPress = recursePredecessors(new GestureInspector() {

			@Override
			public boolean onGestureFound(Gesture g) {
				return g instanceof LongPress;
			}
		});

		if (longPress != null) {
			Control ret = receiver.onContextMenu(getPosition());

			if (ret != null)
				return ret;
		}

		return super.applyTo(receiver);
	}
}
