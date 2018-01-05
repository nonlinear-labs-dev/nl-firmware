package com.nonlinearlabs.NonMaps.client.world.overlay.setup;


public class DeviceNameEditButton extends SetupButton {

	public DeviceNameEditButton(DeviceNameSettings deviceNameSettings) {
		super(deviceNameSettings, "Edit");
	}

	public void applyPadding() {
		double hPadding = (getRelativePosition().getHeight() / 2);
		getRelativePosition().applyPadding(0, hPadding / 2, 0, hPadding / 2);
	}
}
