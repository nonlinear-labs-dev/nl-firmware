package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class SettingsControl extends OverlayLayout {

	public SettingsControl(Control parent) {
		super(parent);
	}

	public abstract void update(Node settingsNode, Node deviceInfo);

}