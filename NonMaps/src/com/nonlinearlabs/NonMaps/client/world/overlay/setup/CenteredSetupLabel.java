package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

class CenteredSetupLabel extends SetupLabel {
	CenteredSetupLabel(OverlayLayout parent, String label) {
		super(parent, label);
	}

	@Override
	protected TextAlign getAlignment() {
		return TextAlign.CENTER;
	}
}