package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class BenderCurve extends Setting {

	int choice = 0;

	protected BenderCurve(DeviceSettingsPage parent) {
		super(parent, "Bender Curve");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().systemSettings.benderCurve.onChange(new Function<Setup.BenderCurve, Boolean>() {

			@Override
			public Boolean apply(Setup.BenderCurve t) {
				choice = t.ordinal();
				return true;
			}
		});
	}

	@Override
	protected OverlayControl createSettingsControl() {
		return new SettingsMenu(this) {

			@Override
			protected String[] getChoices() {
				String ret[] = { "Soft", "Normal", "Hard" };
				return ret;
			}

			@Override
			protected int getChoice() {
				return choice;
			}

			@Override
			protected void chose(int c, boolean fire) {
				choice = c;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);

				if (fire)
					SystemSettings.get().setBenderCurve(com.nonlinearlabs.NonMaps.client.dataModel.Setup.BenderCurve.values()[choice]);
			}

		};
	}
}
