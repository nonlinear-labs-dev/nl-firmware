package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.LinkedList;
import java.util.function.Function;
import java.util.function.Predicate;

public abstract class DataModelEntity<T> implements DataModelEntityBase {
	LinkedList<Function<T, Boolean>> consumers = new LinkedList<Function<T, Boolean>>();
	private T value;

	public DataModelEntity(T def) {
		value = def;
	}

	public void onChange(Function<T, Boolean> cb) {
		consumers.add(cb);
		cb.apply(getValue());
	}

	public void notifyChanges() {
		T v = getValue();

		consumers.removeIf(new Predicate<Function<T, Boolean>>() {

			@Override
			public boolean test(Function<T, Boolean> listener) {
				Boolean needsFurtherUpdates = listener.apply(v);
				return !needsFurtherUpdates;
			}
		});
	}

	public void setValue(T v) {
		if (v != value) {
			value = v;
			notifyChanges();
		}
	}

	public T getValue() {
		return value;
	}
}
