package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.EditParameter;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;
import com.nonlinearlabs.NonMaps.client.world.Control;

public class EditParameterSetting extends Setting {

	private int choice = 0;

	public class EditParameterSettingMenu extends SettingsMenu {
		private EditParameterSettingMenu(Control parent) {
			super(parent);

			Setup.get().localSettings.editParameter.onChange(new Function<Setup.EditParameter, Boolean>() {

				@Override
				public Boolean apply(Setup.EditParameter t) {
					choice = t.ordinal();
					return true;
				}
			});
		}

		@Override
		protected String[] getChoices() {
			String ret[] = { "Always", "If Selected", "Never" };
			return ret;
		}

		@Override
		protected int getChoice() {
			return choice;
		}

		public String getChoiceString() {
			return getChoices()[getChoice()];
		}

		@Override
		protected void chose(int c, boolean fire) {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);

			if (fire)
				LocalSettings.get().setEditParameter(EditParameter.values()[choice]);
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
