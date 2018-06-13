package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class PresetDragDropSetting extends Setting {

	int choice = 0;

	protected PresetDragDropSetting(GUISettingsPage parent) {
		super(parent, "Preset Drag and Drop");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().localSettings.presetDragDrop.onChange(new Function<Setup.BooleanValues, Boolean>() {

			@Override
			public Boolean apply(Setup.BooleanValues t) {
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
					LocalSettings.get().setPresetDragDrop(Setup.BooleanValues.values()[choice]);

			}

		};
	}

	public boolean isEnabled() {
		return choice == 0;
	}
}
