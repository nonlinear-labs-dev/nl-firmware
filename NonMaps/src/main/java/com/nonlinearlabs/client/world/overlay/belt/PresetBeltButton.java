package com.nonlinearlabs.client.world.overlay.belt;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class PresetBeltButton extends BeltButton {

	public PresetBeltButton(OverlayLayout parent, Belt belt) {
		super(parent, belt, "Preset_Tab_Enabled.svg", "Preset_Tab_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		if (!isActive()) {
			belt.setPresetView(true);
		} else {
			belt.toggle();
		}
		return this;
	}

	@Override
	public boolean isActive() {
		return belt.isPresetView();
	}
}
