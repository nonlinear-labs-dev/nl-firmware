package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class SingleSoundSettingsButton extends SVGImage {

	public SingleSoundSettingsButton(Control parent) {
		super(parent, "Append_Enabled.svg");
	}

	public double getPictureHeight() {
		return getPhase(getSelectedPhase()).getImgHeight();
	}

	public double getPictureWidth() {
		return getPhase(getSelectedPhase()).getImgWidth();
	}
}
