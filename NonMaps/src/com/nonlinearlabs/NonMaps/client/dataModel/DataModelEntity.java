package com.nonlinearlabs.NonMaps.client.dataModel;

import java.lang.ref.WeakReference;
import java.util.LinkedList;
import java.util.function.Consumer;

public class DataModelEntity<T> {
	LinkedList<WeakReference<Consumer<T>>> consumers = new LinkedList<WeakReference<Consumer<T>>>();

	public void onChange(Consumer<T> cb) {
		consumers.add(new WeakReference<Consumer<T>>(cb));
	}

	@SuppressWarnings("unchecked")
	public void notifyChanges() {
		for (WeakReference<Consumer<T>> p : consumers) {
			Consumer<T> c = p.get();
			if (c != null) {
				c.accept((T) this);
			}
		}
	}
}
