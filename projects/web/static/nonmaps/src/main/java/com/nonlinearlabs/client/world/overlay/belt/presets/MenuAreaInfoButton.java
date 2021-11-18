package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public abstract class MenuAreaInfoButton extends SVGImage {

	public MenuAreaInfoButton(MenuArea parent) {
		super(parent, "Menu_Info_Enabled_S.svg", "Menu_Info_Active_S.svg", "Menu_Info_Disabled_S.svg");
	}

	@Override
	public MenuArea getParent() {
		return (MenuArea) super.getParent();
	}

	@Override
	public Control click(Position eventPoint) {
		toggle();
		return this;
	}

	public abstract void toggle();
}
