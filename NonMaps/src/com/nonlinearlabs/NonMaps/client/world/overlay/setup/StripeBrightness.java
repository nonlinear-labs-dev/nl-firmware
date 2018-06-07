package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;

public class StripeBrightness extends Setting {

	int choice = 0;

	protected StripeBrightness(GUISettingsPage guiSettingsPage) {
		super(guiSettingsPage, "Stripe Brightness");

		Setup.get().localSettings.stripeBrightness.onChange(new Function<Setup.StripeBrightness, Boolean>() {

			@Override
			public Boolean apply(Setup.StripeBrightness t) {
				choice = t.ordinal();
				return true;
			}
		});
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new SettingsMenu(this) {

			@Override
			protected String[] getChoices() {
				String ret[] = { "Off", "10%", "25%", "50%" };
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
					LocalSettings.get().setStripeBrightness(Setup.StripeBrightness.values()[choice]);
			}
		};
	}

}
