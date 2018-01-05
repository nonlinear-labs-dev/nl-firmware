package com.nonlinearlabs.NonMaps.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class GlobalMenuHeaderLabel extends Label {

	public GlobalMenuHeaderLabel(OverlayLayout parent) {
		super(parent);
	}
	@Override
	protected double getFontHeight(Rect pixRect) {
		return 18;
	}
	@Override
	public String getDrawText(Context2d ctx) {
		return "View";
	}
	@Override
	protected RGB getLineColor()
	{
		return new RGB(155, 199, 236);
	}

}
