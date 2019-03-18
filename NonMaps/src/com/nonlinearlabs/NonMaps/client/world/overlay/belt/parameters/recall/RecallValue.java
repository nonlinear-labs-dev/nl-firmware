package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class RecallValue extends Label {

	protected boolean active = false;
	
	public void setActive(boolean b) {
		active = b;
	}
	
	public boolean isActive() {
		return active;
	}
	
	public RecallValue(OverlayLayout parent) {
		super(parent);
		setFontHeightInMM(5);
		setFontColor(new Gray(155));
	}

	public double getFontSizeInPixel() {
		return getFontHeight(getPixRect());
	}

	public double calculateTextWidth() {
		Context2d ctx = NonMaps.get().getCanvas().getContext2d();
		String text = getDrawText(ctx);
		ctx.setFont(getFontSizeInPixel() + "px 'Source Sans Pro LW25'");		
		return Millimeter.toPixels(2) + ctx.measureText(text).getWidth() + Millimeter.toPixels(2);
	}
}