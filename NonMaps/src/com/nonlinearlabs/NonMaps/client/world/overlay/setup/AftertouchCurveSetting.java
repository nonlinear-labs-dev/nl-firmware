package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup;

public class AftertouchCurveSetting extends Setting {

	int choice = 0;

	protected AftertouchCurveSetting(DeviceSettingsPage parent) {
		super(parent, "Aftertouch Curve");

		Setup.get().systemSettings.aftertouchCurve.onChange(new Function<Setup.AftertouchCurve, Boolean>() {

			@Override
			public Boolean apply(Setup.AftertouchCurve t) {
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
				String ret[] = { "Soft", "Normal", "Hard" };
				return ret;
			}

			@Override
			protected int getChoice() {
				return choice;
			}

			@Override
			protected void chose(int c, boolean sendToServer) {
				choice = c;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);

				if (sendToServer)
					NonMaps.theMaps.getServerProxy().setSetting("AftertouchCurve", getSettingsValueString(c));
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

		};
	}
}
