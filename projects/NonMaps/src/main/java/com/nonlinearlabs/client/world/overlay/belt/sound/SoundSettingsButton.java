package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettingsDialog;

public class SoundSettingsButton extends SVGImage {

	public SoundSettingsButton(Control parent) {
		super(parent, "More_A_Enabled.svg");
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
