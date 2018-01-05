package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

class InitSoundButton extends SoundBeltButton {
	InitSoundButton(OverlayLayout parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return "Initialize Sound";
	}

	@Override
	public Control click(Position eventPoint) {
		NonMaps.theMaps.getServerProxy().initSound();
		return this;
	}
}