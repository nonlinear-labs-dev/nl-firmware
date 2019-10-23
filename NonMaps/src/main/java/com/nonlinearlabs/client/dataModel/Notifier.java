package com.nonlinearlabs.client.dataModel;

import java.util.LinkedList;
import java.util.function.Function;
import java.util.function.Predicate;

public abstract class Notifier<T> {

	LinkedList<Function<T, Boolean>> consumers = new LinkedList<Function<T, Boolean>>();

	public void onChange(Function<T, Boolean> cb) {
		consumers.add(cb);
		cb.apply(getValue());
	}

	public boolean notifyChanges() {
		T v = getValue();

		consumers.removeIf(new Predicate<Function<T, Boolean>>() {

			@Override
			public boolean test(Function<T, Boolean> listener) {
				Boolean needsFurtherUpdates = listener.apply(v);
				return !needsFurtherUpdates;
			}
		});

		return true;
	}

	public abstract T getValue();
}