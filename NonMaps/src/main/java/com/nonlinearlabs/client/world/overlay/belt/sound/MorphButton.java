package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class MorphButton extends SoundBeltButton {

	public MorphButton(OverlayLayout parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return "Morph";
	}

	@Override
	public RGB getColorFont() {
		return super.getColorFont().darker(80);
	}
}
