package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;

public class BitmapCache extends Setting {

	int choice = 0;

	protected BitmapCache(GUISettingsPage parent) {
		super(parent, "Bitmap Cache");
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
				choice = c;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);

				if (sendToServer)
					NonMaps.theMaps.getNonLinearWorld().getSettings().set("BitmapCache", getSettingsValueString(c));
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

			@Override
			public void update(Node settingsNode, Node deviceInfo) {
				String str = NonMaps.theMaps.getNonLinearWorld().getSettings().get("BitmapCache", "on");
				fromSettingsString(str);
			}
		};
	}
}
