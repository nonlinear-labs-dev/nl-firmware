package com.nonlinearlabs.client.world.pointer;

import java.util.function.Consumer;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.nonlinearlabs.client.world.Position;

public class DoubleClickWaiter {

	private boolean canceled = false;

	public DoubleClickWaiter(Position p, Consumer<Position> c) {

		Scheduler.get().scheduleFixedDelay(new RepeatingCommand() {

			@Override
			public boolean execute() {
				if (!canceled)
					c.accept(p);
				return false;
			}
		}, 250);
	}

	public void onDoubleClick() {
		cancel();
	}

	public void cancel() {
		canceled = true;
	}
}
