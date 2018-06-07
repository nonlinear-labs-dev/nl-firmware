package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;

public class SelectionAutoScrollSetting extends Setting {

	int choice = 0;

	protected SelectionAutoScrollSetting(GUISettingsPage parent) {
		super(parent, "Selection Auto Scroll");

		Setup.get().localSettings.selectionAutoScroll.onChange(new Function<Setup.SelectionAutoScroll, Boolean>() {

			@Override
			public Boolean apply(Setup.SelectionAutoScroll t) {
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
				String ret[] = { "Off", "Parameter", "Preset", "Parameter and Preset" };
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
					LocalSettings.get().setSelectionAutoScroll(Setup.SelectionAutoScroll.values()[choice]);
			}
		};
	}
}
