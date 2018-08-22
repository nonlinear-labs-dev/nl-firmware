package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class ShowDeveloperOptions extends Setting {

	int choice = 0;

	protected ShowDeveloperOptions(GUISettingsPage parent) {
		super(parent, "Show Developer Options");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().localSettings.showDeveloperOptions.onChange(new Function<Setup.BooleanValues, Boolean>() {

			@Override
			public Boolean apply(Setup.BooleanValues t) {
				choice = t.ordinal();
				NonMaps.theMaps.getNonLinearWorld().getSettings().setVisible(t == BooleanValues.on);
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
				if (choice != c) {
					if (fire) {
						showAlerts(c);
					}
				}

				choice = c;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);

				if (fire)
					LocalSettings.get().showDeveloperOptions(Setup.BooleanValues.values()[choice]);

				if (choice == 1 && fire)
					NonMaps.theMaps.getNonLinearWorld().getSettings().setToDefaults();
			}

			private void showAlerts(int c) {
				if (c == 0) {
					Window.alert("For developers only!");
				} else if (c == 1) {
					Window.alert("Changes of the developer options will be discarded.");
				}
			}
		};
	}
}
