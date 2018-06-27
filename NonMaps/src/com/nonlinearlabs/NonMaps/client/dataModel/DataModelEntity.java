package com.nonlinearlabs.NonMaps.client.dataModel;

public abstract class DataModelEntity<T> extends Notifier<T> implements DataModelEntityBase {
	private T value;

	public DataModelEntity(T def) {
		value = def;
	}

	public boolean setValue(T v) {
		if (v != value) {
			value = v;
			notifyChanges();
			return true;
		}
		return false;
	}

	@Override
	public T getValue() {
		return value;
	}
}
