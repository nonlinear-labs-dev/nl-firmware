package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SearchQueryDialog;

public class MenuAreaSearchButton extends MenuAreaButton {

	public MenuAreaSearchButton(MenuArea parent) {
		super(parent, "Menu_Search_Disabled_S.svg", "Menu_Search_Enabled_S.svg", "Menu_Search_Active_S.svg", "Menu_Search_Disabled_L.svg",
				"Menu_Search_Enabled_L.svg", "Menu_Search_Active_L.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		toggleSearch();
		return this;
	}

	private void toggleSearch() {
		SearchQueryDialog.toggle();
	}

	@Override
	protected State getState() {
		if (getPresetManager().isEmpty())
			return State.Disabled;

		return SearchQueryDialog.isShown() ? State.Active : State.Enabled;
	}

}
