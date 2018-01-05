package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.i18n.client.NumberFormat;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;

public class DisplayScalingFactor extends Setting {

	int choice = 0;

	protected DisplayScalingFactor(GUISettingsPage guiSettingsPage) {
		super(guiSettingsPage, "Display Scaling Factor");
	}

	@Override
	protected SettingsControl createSettingsControl() {
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

				Number percent = NumberFormat.getPercentFormat().parse(getCurrentChoiceString());
				getNonMaps().setPixelFactor(percent.doubleValue() / 100);

				if (sendToServer)
					NonMaps.theMaps.getNonLinearWorld().getSettings().set("UI Scaling", getSettingsValueString(c));
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

			@Override
			public void update(Node settingsNode, Node deviceInfo) {
				String str = NonMaps.theMaps.getNonLinearWorld().getSettings().get("UI Scaling", "100%");
				fromSettingsString(str);
			}
		};
	}

}
