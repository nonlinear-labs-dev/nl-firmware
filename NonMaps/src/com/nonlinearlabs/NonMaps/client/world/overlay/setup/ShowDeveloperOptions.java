package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.user.client.Window;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;

public class ShowDeveloperOptions extends Setting {

	int choice = 0;

	protected ShowDeveloperOptions(GUISettingsPage parent) {
		super(parent, "Show Developer Options");
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
			protected void chose(int c, boolean sendToServer) {
				if (choice != c) {
					if (sendToServer) {
						showAlerts(c);
					}
				}

				choice = c;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);

				if (sendToServer)
					NonMaps.theMaps.getNonLinearWorld().getSettings().set("ShowDeveloperOptions", getSettingsValueString(c));

				setDeveloperOptionsVisibility();

				if (choice == 1 && sendToServer)
					NonMaps.theMaps.getNonLinearWorld().getSettings().setToDefaults();
			}

			private void showAlerts(int c) {
				if (c == 0) {
					Window.alert("For developers only!");
				} else if (c == 1) {
					Window.alert("Changes of the developer options will be discarded.");
				}
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

			@Override
			public void update(Node settingsNode, Node deviceInfo) {
				String str = setDeveloperOptionsVisibility();
				fromSettingsString(str);
			}

			private String setDeveloperOptionsVisibility() {
				String str = NonMaps.theMaps.getNonLinearWorld().getSettings().get("ShowDeveloperOptions", "off");
				NonMaps.theMaps.getNonLinearWorld().getSettings().setVisible(str.equals("on"));
				return str;
			}
		};
	}
}
