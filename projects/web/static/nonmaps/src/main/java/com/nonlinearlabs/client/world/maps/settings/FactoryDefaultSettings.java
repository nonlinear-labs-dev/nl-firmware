package com.nonlinearlabs.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

public class FactoryDefaultSettings extends Setting {

	protected FactoryDefaultSettings(DeveloperSettings parent) {
		super(parent, "Load Factory Defaults", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
        NonMaps.get().getServerProxy().factoryDefaults();
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
