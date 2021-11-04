package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;

public class PresetColorPack {
	public PresetColorPack(RGB c, RGB f, RGB h) {
		contour = c;
		fill = f;
		highlight = h;
		overlay = new RGBA(0, 0, 0, 0);
	}

	public PresetColorPack(PresetColorPack a) {
		contour = a.contour.clone();
		fill = a.fill.clone();
		highlight = a.highlight.clone();
		overlay = a.overlay.clone();
	}

	public PresetColorPack applyNotMatched() {
		PresetColorPack ret = new PresetColorPack(contour.clone(), fill.clone(), highlight.clone());
		ret.overlay = new RGBA(0, 0, 0, 0.75);
		return ret;
	}

	public RGB contour;
	public RGB fill;
	public RGB highlight;
	public RGBA overlay;
}