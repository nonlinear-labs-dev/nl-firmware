package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SetupPage extends OverlayLayout {

	protected SetupPage(SetupPages parent) {
		super(parent);
	}

	public void update(Node settingsNode, Node deviceInfo) {
		for (OverlayControl c : getChildren()) {
			if (c instanceof Setting) {
				Setting s = (Setting) c;
				s.update(settingsNode, deviceInfo);
			}
		}
	}
}
