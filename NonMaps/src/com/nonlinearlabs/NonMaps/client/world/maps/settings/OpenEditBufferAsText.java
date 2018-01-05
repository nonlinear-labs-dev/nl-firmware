package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;

public class OpenEditBufferAsText extends Setting {

	protected OpenEditBufferAsText(DeveloperSettings parent) {
		super(parent, "Open Edit Buffer as Text", "Go!");
	}

	@Override
	public Control onValueButtonMouseDown(Position eventPoint) {
		Window.open("/presets/param-editor/open-editbuffer-as-text", "", "");
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
