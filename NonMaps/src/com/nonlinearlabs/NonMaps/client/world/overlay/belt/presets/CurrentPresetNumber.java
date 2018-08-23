package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class CurrentPresetNumber extends Label {

	public CurrentPresetNumber(OverlayLayout parent) {
		super(parent);
		super.setFontColor(new RGB(120, 120, 120));
		super.setFontHeightInMM(5);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return NonMaps.theMaps.getNonLinearWorld().getPresetManager().getLoadedPresetNumberString();
	}

	public double getDesiredWidth() {
		return Millimeter.toPixels(35);
	}

	public double getDesiredHeight() {
		return Millimeter.toPixels(20);
	}

	@Override
	protected double getVerticalFontDisplacement() {
		return Millimeter.toPixels(4);
	}

	@Override
	public RGB getColorFont() {
		return new RGB(255, 255, 255);
	}

	@Override
	protected RGB getColorFontForSplit(int i) {
		if (i == 0)
			return getLineColor();

		return new RGB(250, 250, 250);
	}
}
