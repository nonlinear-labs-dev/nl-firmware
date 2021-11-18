package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.i18n.client.NumberFormat;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.client.world.overlay.Label;

public class PresetNumber extends Label {

	public PresetNumber(BeltPreset parent) {
		super(parent);
	}

	@Override
	public BeltPreset getParent() {
		return (BeltPreset) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		Preset p = getParent().getMapsPreset();

		if (p != null) {
			int presetNumber = p.getNumber();
			return NumberFormat.getFormat("#000").format(presetNumber);
		}

		return "";
	}

	protected TextAlign getAlignment() {
		return TextAlign.RIGHT;
	}
}
