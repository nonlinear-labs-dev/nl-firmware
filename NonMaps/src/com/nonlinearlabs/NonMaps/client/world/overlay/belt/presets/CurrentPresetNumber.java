package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class CurrentPresetNumber extends Label {

	public CurrentPresetNumber(OverlayLayout parent) {
		super(parent);
		super.setFontColor(new RGB(120,120,120));
		super.setFontHeightInPx(20);	}

	@Override
	public String getDrawText(Context2d ctx) {
		return NonMaps.theMaps.getNonLinearWorld().getPresetManager().getLoadedPresetNumberString();
	}

}
