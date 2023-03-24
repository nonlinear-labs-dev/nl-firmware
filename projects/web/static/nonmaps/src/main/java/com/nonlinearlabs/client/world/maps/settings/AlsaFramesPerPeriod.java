package com.nonlinearlabs.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

class AlsaFramesPerPeriod extends Setting {

	AlsaFramesPerPeriod(DeveloperSettings parent) {
		super(parent, "Alsa Frames per Period", "Set");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
		String v = Window.prompt("Frames per Period (16 = default, 1...512)", "16");
		getNonMaps().getServerProxy().setSetting("AlsaFramesPerPeriod", v);
		return this;
	}

	@Override
	public ContextMenu createMenu(NonPosition pos) {
		return null;
	}

	@Override
	public void setDefault() {
	}

}
