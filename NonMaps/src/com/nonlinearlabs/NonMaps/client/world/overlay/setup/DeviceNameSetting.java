package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.Control;

public class DeviceNameSetting extends Setting {

	public DeviceNameSetting(Control parent) {
		super(parent, "Device Name");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new DeviceNameSettings(this);
	}

}
