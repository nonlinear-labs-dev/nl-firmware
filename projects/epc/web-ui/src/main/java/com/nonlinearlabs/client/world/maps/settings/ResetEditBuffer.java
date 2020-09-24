package com.nonlinearlabs.client.world.maps.settings;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

class ResetEditBuffer extends Setting {

	ResetEditBuffer(DeveloperSettings parent) {
		super(parent, "Init Sound", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
		getNonMaps().getServerProxy().resetEditBuffer();
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
