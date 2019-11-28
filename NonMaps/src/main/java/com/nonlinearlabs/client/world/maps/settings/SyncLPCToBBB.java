package com.nonlinearlabs.client.world.maps.settings;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

class SyncLPCToBBB extends Setting {

	SyncLPCToBBB(DeveloperSettings parent) {
		super(parent, "Sync LPC to BBB", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
		getNonMaps().getServerProxy().syncLPCToBBB();
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