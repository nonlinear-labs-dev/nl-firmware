package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;

public class ButtonRepeat {

	private static ButtonRepeat br = new ButtonRepeat();
	private Runnable runnable;

	private ButtonRepeat() {

	}

	public void start(final Runnable r) {
		runnable = r;
		r.run();
		scheduleFirstRepeat(r);
	}

	public boolean isRunning() {
		return runnable != null;
	}

	private void scheduleFirstRepeat(final Runnable r) {
		Scheduler.get().scheduleFixedDelay(new RepeatingCommand() {

			@Override
			public boolean execute() {
				if (r == runnable) {
					r.run();
					scheduleNextRepeats(r);
				}
				return false;
			}
		}, 500);
	}

	protected void scheduleNextRepeats(final Runnable r) {
		Scheduler.get().scheduleFixedDelay(new RepeatingCommand() {

			@Override
			public boolean execute() {
				if (r == runnable) {
					r.run();
					return true;
				}
				return false;
			}
		}, 200);

	}

	public void cancel() {
		runnable = null;
	}

	public static ButtonRepeat get() {
		return br;
	}
}
