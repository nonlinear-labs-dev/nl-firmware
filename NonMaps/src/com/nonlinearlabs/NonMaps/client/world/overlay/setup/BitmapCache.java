package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;

public class BitmapCache extends Setting {

	int choice = 0;

	protected BitmapCache(GUISettingsPage parent) {
		super(parent, "Bitmap Cache");

		Setup.get().localSettings.bitmapCache.onChange(new Function<Setup.BooleanValues, Boolean>() {

			@Override
			public Boolean apply(Setup.BooleanValues t) {
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
				String ret[] = { "On", "Off" };
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
					LocalSettings.get().setBitmapCache(Setup.BooleanValues.values()[choice]);
			}
		};
	}
}
