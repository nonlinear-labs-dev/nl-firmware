package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;

public class ContextMenusSetting extends Setting {

	int choice = 0;

	protected ContextMenusSetting(GUISettingsPage parent) {
		super(parent, "Context Menus");
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
					getNonMaps().getServerProxy().setSetting("ContextMenus", getSettingsValueString(c));
			}

			protected String getSettingsValueString(int c) {
				return getChoices()[c].replace(" ", "-").toLowerCase();
			}

			@Override
			public void update(Node settingsNode, Node deviceInfo) {
				String str = ServerProxy.getChildText(settingsNode, "ContextMenus", "value");
				fromSettingsString(str);
			}
		};
	}

	public boolean isEnabled() {
		return choice == 0;
	}
}
