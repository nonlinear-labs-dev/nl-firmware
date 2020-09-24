package com.nonlinearlabs.client.world.overlay;

import com.google.gwt.canvas.dom.client.Context2d;

abstract class TextLabel extends Label {

	private String text = "";

	TextLabel(OverlayLayout parent, String text) {
		super(parent);
		setText(text);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return text;
	}

	public void setText(String text) {
		if (text == null)
			text = "";

		if (!this.text.equals(text)) {
			this.text = text;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}
}
