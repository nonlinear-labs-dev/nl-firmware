package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.DisplayScaling;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class DisplayScalingFactor extends Setting {

	int choice = 0;

	protected DisplayScalingFactor(GUISettingsPage guiSettingsPage) {
		super(guiSettingsPage, "Display Scaling Factor");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().localSettings.displayScaling.onChange(new Function<Setup.DisplayScaling, Boolean>() {

			@Override
			public Boolean apply(Setup.DisplayScaling t) {
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
				String ret[] = { "50%", "75%", "100%", "125%", "150%" };
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
					LocalSettings.get().setDisplayScaling(DisplayScaling.values()[choice]);
			}
		};
	}

}
