package com.nonlinearlabs.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

public class StartBBBWiFiForEPC2 extends Setting {

	protected StartBBBWiFiForEPC2(DeveloperSettings parent) {
		super(parent, "Enable BBB wifi on EPC2 Devices", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
        NonMaps.get().getServerProxy().startBBBWifiIfEPC2();
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