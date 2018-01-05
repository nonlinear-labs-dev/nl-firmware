package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class OSVersion extends Setting {

	public OSVersion(SystemInfoPage systemInfoPage) {
		super(systemInfoPage, "OS Version");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new SettingsNodeConnectedLabel(this) {

			@Override
			public String getLabelText(Node settingsNode, Node deviceInfo) {
				return ServerProxy.getChildText(deviceInfo, "os-version");
			}
		};
	}
}
