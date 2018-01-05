package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;

public class EditParameterSetting extends Setting {

	public class EditParameterSettingMenu extends SettingsMenu {
		private EditParameterSettingMenu(Control parent) {
			super(parent);
		}

		@Override
		protected String[] getChoices() {
			String ret[] = { "Always", "If Selected", "Never" };
			return ret;
		}

		@Override
		protected int getChoice() {
			String choice = NonMaps.theMaps.getNonLinearWorld().getSettings().get("ParameterDrag", getSettingsValueString(0));
			int ret = settingsStringToIndex(choice);
			if (ret >= 0)
				return ret;

			return 0;
		}

		public String getChoiceString() {
			return getChoices()[getChoice()];
		}

		@Override
		protected void chose(int c, boolean sendToServer) {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);

			if (sendToServer)
				NonMaps.theMaps.getNonLinearWorld().getSettings().set("ParameterDrag", getSettingsValueString(c));
		}

		protected String getSettingsValueString(int c) {
			return getChoices()[c].replace(" ", "-").toLowerCase();
		}

		@Override
		public void update(Node settingsNode, Node deviceInfo) {
		}
	}

	protected EditParameterSetting(GUISettingsPage parent) {
		super(parent, "Edit Parameter");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new EditParameterSettingMenu(this);
	}

	@Override
	public EditParameterSettingMenu getSettingsControl() {
		return (EditParameterSettingMenu) super.getSettingsControl();
	}
}
