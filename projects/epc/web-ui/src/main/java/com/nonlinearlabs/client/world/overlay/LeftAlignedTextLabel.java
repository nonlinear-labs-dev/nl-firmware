package com.nonlinearlabs.client.world.overlay;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;

class LeftAlignedTextLabel extends TextLabel {
	LeftAlignedTextLabel(OverlayLayout parent, String text) {
		super(parent, text);
	}

	@Override
	protected TextAlign getAlignment() {
		return TextAlign.LEFT;
	}
}