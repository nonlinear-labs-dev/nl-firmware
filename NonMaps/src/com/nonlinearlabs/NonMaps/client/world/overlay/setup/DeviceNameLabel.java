package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;

public class DeviceNameLabel extends SetupLabel {

	public DeviceNameLabel(DeviceNameSettings deviceNameSettings) {
		super(deviceNameSettings, "---");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().systemSettings.deviceName.onChange(new Function<String, Boolean>() {

			@Override
			public Boolean apply(String v) {
				setText(v);
				return true;
			}
		});
	}
}
