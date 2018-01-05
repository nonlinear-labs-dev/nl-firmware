package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class VelocityCurveSetting extends Setting {

	int choice = 0;

	protected VelocityCurveSetting(DeviceSettingsPage parent) {
		super(parent, "Velocity Curve");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new SettingsMenu(this) {

			@Override
			protected String[] getChoices() {
				String ret[] = { "Very Soft", "Soft", "Normal", "Hard", "Very Hard" };
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
					NonMaps.theMaps.getServerProxy().setSetting("VelocityCurve", getSettingsValueString(c));
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

			@Override
			public void update(Node settingsNode, Node deviceInfo) {
				String str = ServerProxy.getChildText(settingsNode, "VelocityCurve", "value");
				fromSettingsString(str);
			}
		};
	}
}
