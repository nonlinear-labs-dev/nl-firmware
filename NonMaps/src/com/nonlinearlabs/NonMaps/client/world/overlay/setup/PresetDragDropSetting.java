package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class PresetDragDropSetting extends Setting {

	int choice = 0;

	protected PresetDragDropSetting(GUISettingsPage parent) {
		super(parent, "Preset Drag and Drop");
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
					getNonMaps().getServerProxy().setSetting("PresetDragDropEnabled", getSettingsValueString(c));
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

			@Override
			public void update(Node settingsNode, Node deviceInfo) {
				String str = ServerProxy.getChildText(settingsNode, "PresetDragDropEnabled", "value");
				fromSettingsString(str);
			}
		};
	}

	public boolean isEnabled() {
		return choice == 0;
	}
}
