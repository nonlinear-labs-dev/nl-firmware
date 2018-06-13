package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.DeviceInfomation;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class UISoftwareVersion extends Setting {

	public UISoftwareVersion(SystemInfoPage systemInfoPage) {
		super(systemInfoPage, "UI Software Version");
	}

	@Override
	public void init() {
		super.init();

		DeviceInfomation.get().softwareVersion.onChange(new Function<String, Boolean>() {

			@Override
			public Boolean apply(String t) {
				((SetupLabel) getSettingsControl()).setText(t);
				return true;
			}
		});
	}

	@Override
	protected OverlayControl createSettingsControl() {
		return new SetupLabel(this, "---");
	}
}
