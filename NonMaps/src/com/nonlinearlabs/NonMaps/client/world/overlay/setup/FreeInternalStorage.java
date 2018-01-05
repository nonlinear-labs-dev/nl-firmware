package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class FreeInternalStorage extends Setting {

	public FreeInternalStorage(SystemInfoPage systemInfoPage) {
		super(systemInfoPage, "Free Internal Memory");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new SettingsNodeConnectedLabel(this) {

			@Override
			public String getLabelText(Node settingsNode, Node deviceInfo) {
				return ServerProxy.getChildText(deviceInfo, "free-disc-space");
			}
		};
	}
}
