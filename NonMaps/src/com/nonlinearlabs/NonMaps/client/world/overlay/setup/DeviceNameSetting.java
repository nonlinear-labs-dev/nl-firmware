package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class DeviceNameSetting extends Setting {

	public DeviceNameSetting(Control parent) {
		super(parent, "Device Name");
	}

	@Override
	protected OverlayControl createSettingsControl() {
		return new DeviceNameSettings(this);
	}

}
