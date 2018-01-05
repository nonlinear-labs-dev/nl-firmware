package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class DeviceNameLabel extends SetupLabel {

	public DeviceNameLabel(DeviceNameSettings deviceNameSettings) {
		super(deviceNameSettings, "---");
	}

	public void update(Node settingsNode) {
		String str = ServerProxy.getChildText(settingsNode, "DeviceName", "value");

		if (str != null && !str.isEmpty())
			setText(str);
	}
}
