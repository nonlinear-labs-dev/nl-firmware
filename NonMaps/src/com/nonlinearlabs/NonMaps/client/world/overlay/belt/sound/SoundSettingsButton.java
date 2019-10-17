package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.html.soundSettings.SoundSettingsDialog;

public class SoundSettingsButton extends SVGImage {

	public SoundSettingsButton(Control parent) {
		super(parent, "Append_Enabled.svg");
	}

	public double getPictureHeight() {
		return getPhase(getSelectedPhase()).getImgHeight();
	}

	public double getPictureWidth() {
		return getPhase(getSelectedPhase()).getImgWidth();
	}

	@Override
	public Control click(Position eventPoint) {
		SoundSettingsDialog.toggle();
		return this;
	}
}
