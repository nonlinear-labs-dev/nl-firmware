package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.storage.client.Storage;
import com.google.gwt.storage.client.StorageMap;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class DeveloperSettings extends LayoutResizingVertical {

	private SendPresetAsLPCWriteFallback sendPresetAsLPCWriteFallback;
	private DebugLevel debugLevel;
	private Storage localStorage;
	private StorageMap keyValueStore;
	private KioskMode kioskMode;
	private BlockingMainThreadIndication blockingMainThreadIndication;

	public DeveloperSettings(MapsLayout parent) {
		super(parent);

		localStorage = Storage.getLocalStorageIfSupported();

		if (localStorage != null)
			keyValueStore = new StorageMap(localStorage);

		addChild(new SetupHeader(this));
		sendPresetAsLPCWriteFallback = addChild(new SendPresetAsLPCWriteFallback(this));
		addChild(new ResetEditBuffer(this));
		addChild(new SyncLPCToBBB(this));
		debugLevel = addChild(new DebugLevel(this));
		addChild(new UploadPreset(this));
		addChild(new DownloadEditBufferAsText(this));
		addChild(new DownloadErrorReport(this));
		addChild(new OpenEditBufferAsText(this));
		addChild(new TestHWUIDisplay(this));
		kioskMode = addChild(new KioskMode(this));
		blockingMainThreadIndication = addChild(new BlockingMainThreadIndication(this));
	}

	public String get(String key, String def) {
		if (keyValueStore != null) {
			if (keyValueStore.containsValue(key) != true) {
				String r = keyValueStore.get(key);
				if (r != null)
					return r;
			}
		}

		return def;
	}

	public void set(String key, String val) {
		if (keyValueStore != null)
			keyValueStore.put(key, val);
	}

	public void update(Node settingsNode, Node presetManagerNode, Node deviceInfo) {
		sendPresetAsLPCWriteFallback.update(settingsNode);
		kioskMode.update(settingsNode);
		debugLevel.update(settingsNode);
		blockingMainThreadIndication.update(settingsNode);
	}

	public boolean isOneOf(String key, String... choices) {
		String v = get(key, "");

		for (String c : choices)
			if (c.equals(v))
				return true;

		return false;
	}

	public void setToDefaults() {
		for (MapsControl c : getChildren()) {
			if (c instanceof Setting)
				((Setting) c).setDefault();
		}

	}
}
