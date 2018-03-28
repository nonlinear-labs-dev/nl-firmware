package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;

public class SystemDateTimeInfo extends Setting {

	public SystemDateTimeInfo(Control parent) {
		super(parent, "Date/Time");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new SettingsNodeConnectedLabel(this) {

			@Override
			public String getLabelText(Node settingsNode, Node deviceInfo) {
				return ServerProxy.getChildText(deviceInfo, "date-time-display");
			}
		};
	}

}
