package com.nonlinearlabs.client.localStorage;

import com.google.gwt.storage.client.Storage;
import com.google.gwt.storage.client.StorageMap;
import com.nonlinearlabs.client.useCases.LocalStorage;

public class WebStorage implements LocalStorage {

	private Storage localStorage;
	private StorageMap keyValueStore;

	public WebStorage() {
		localStorage = Storage.getLocalStorageIfSupported();

		if (localStorage != null)
			keyValueStore = new StorageMap(localStorage);
	}

	@Override
	public void store(String key, String value) {
		keyValueStore.put(key, value);
	}

	@Override
	public String load(String key, String defaultValue) {
		if (keyValueStore != null) {
			if (keyValueStore.containsValue(key) != true) {
				String r = keyValueStore.get(key);
				if (r != null)
					return r;
			}
		}

		return defaultValue;
	}
}
