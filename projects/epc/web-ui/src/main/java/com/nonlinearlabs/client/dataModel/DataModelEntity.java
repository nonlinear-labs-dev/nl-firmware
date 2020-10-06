package com.nonlinearlabs.client.dataModel;

public abstract class DataModelEntity<T> extends Notifier<T> implements DataModelEntityBase {
	private T value;

	public DataModelEntity(T def) {
		value = def;
	}

	public boolean setValue(T v) {
		if (!v.equals(value) || forceNotify) {
			value = v;
			notifyChanges();
			return true;
		}
		return false;
	}

	public void notifyOnEachSet(boolean n) {
		forceNotify = n;
	}

	private boolean forceNotify = false;

	@Override
	public T getValue() {
		return value;
	}
}
