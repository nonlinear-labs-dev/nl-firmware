package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class PresetGlitchSetting extends Setting {

	int choice = 0;

	protected PresetGlitchSetting(DeviceSettingsPage parent) {
		super(parent, "Preset Glitch Suppression");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().systemSettings.presetGlitchSuppression.onChange(new Function<Setup.BooleanValues, Boolean>() {

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
					SystemSettings.get().setPresetGlitchSurpression(BooleanValues.values()[choice]);
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

		};
	}
}
