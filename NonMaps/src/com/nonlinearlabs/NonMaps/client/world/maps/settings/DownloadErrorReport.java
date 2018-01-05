package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;

public class DownloadErrorReport extends Setting {

	protected DownloadErrorReport(DeveloperSettings parent) {
		super(parent, "Download Error Report", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
		Window.open("/C15-journal.tar.gz", "", "");
		return this;
	}

	@Override
	public ContextMenu createMenu(NonPosition pos) {
		return null;
	}

	@Override
	public void setDefault() {
		// TODO Auto-generated method stub

	}
}
