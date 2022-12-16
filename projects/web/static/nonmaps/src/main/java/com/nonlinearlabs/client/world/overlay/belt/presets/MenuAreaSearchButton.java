package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class MenuAreaSearchButton extends SVGImage {

	public MenuAreaSearchButton(MenuArea parent) {
		super(parent, "Menu_Search_Enabled_S.svg", "Menu_Search_Active_S.svg", "Menu_Search_Disabled_S.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		toggleSearch();
		return this;
	}

	private void toggleSearch() {
		NonMaps.togglePresetSearch();
	}

	@Override
	public int getSelectedPhase() {
		if (NonMaps.get().getNonLinearWorld().getPresetManager().isEmpty())
			return drawStates.disabled.ordinal();

		return NonMaps.isPresetSearchVisible() ? drawStates.active.ordinal() : drawStates.normal.ordinal();
	}
}
