package com.nonlinearlabs.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

public class DownloadEditBufferAsText extends Setting {

	protected DownloadEditBufferAsText(DeveloperSettings parent) {
		super(parent, "Download Edit Buffer as Text", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
		Window.open("/param-editor/download-editbuffer-as-text", "", "");
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
