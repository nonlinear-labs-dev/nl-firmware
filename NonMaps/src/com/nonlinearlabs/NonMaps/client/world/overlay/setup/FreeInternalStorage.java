package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.DeviceInfomation;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class FreeInternalStorage extends Setting {

	public FreeInternalStorage(SystemInfoPage systemInfoPage) {
		super(systemInfoPage, "Free Internal Memory");
	}

	@Override
	public void init() {
		super.init();

		DeviceInfomation.get().freeDiscSpace.onChange(new Function<String, Boolean>() {

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
