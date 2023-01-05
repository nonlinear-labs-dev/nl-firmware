package com.nonlinearlabs.client.dataModel;

import java.util.LinkedList;
import java.util.function.Function;
import java.util.function.Predicate;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.ScheduledCommand;

public abstract class Notifier<T> {

	static boolean debug = false;

	static native boolean getTestingFlag() /*-{
		return window['isCurrentlyTesting'] == 1;
	}-*/;

	private LinkedList<Function<T, Boolean>> consumers = new LinkedList<Function<T, Boolean>>();
	private boolean scheduled = false;

	public void onChange(Function<T, Boolean> cb) {
		consumers.add(cb);
		cb.apply(getValue());
	}

	public boolean notifyChanges() {
		if (debug || getTestingFlag()) {
			notifyNow();
			return true;
		}

		if (!scheduled) {
			scheduled = true;

			Scheduler.get().scheduleDeferred(new ScheduledCommand() {

				@Override
				public void execute() {
					notifyNow();
				}

			});
		}
		return true;
	}

	private void notifyNow() {
		scheduled = false;
		T v = getValue();

		consumers.removeIf(new Predicate<Function<T, Boolean>>() {

			@Override
			public boolean test(Function<T, Boolean> listener) {
				Boolean needsFurtherUpdates = listener.apply(v);
				return !needsFurtherUpdates;
			}
		});
	}

	public abstract T getValue();
}