package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset;

import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;

public class PresetColorPack {
	public PresetColorPack(RGB c, RGB f, RGB h, double strokeWidth) {
		contour = c;
		fill = f;
		highlight = h;
		highlightStrokeWidth = strokeWidth;
		overlay = new RGBA(0, 0, 0, 0);
	}

	public PresetColorPack(PresetColorPack a) {
		contour = a.contour.clone();
		fill = a.fill.clone();
		highlight = a.highlight.clone();
		highlightStrokeWidth = a.highlightStrokeWidth;
		overlay = a.overlay.clone();
	}

	public PresetColorPack applyNotMatched() {
		PresetColorPack ret = new PresetColorPack(contour.clone(), fill.clone(), highlight.clone(), 1);
		ret.overlay = new RGBA(0, 0, 0, 0.75);
		return ret;
	}

	public RGB contour;
	public RGB fill;
	public RGB highlight;
	public double highlightStrokeWidth;
	public RGBA overlay;
}