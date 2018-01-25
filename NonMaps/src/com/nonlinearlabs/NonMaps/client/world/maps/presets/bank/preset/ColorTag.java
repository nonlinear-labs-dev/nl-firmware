package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ZoomReactingControl;

public class ColorTag extends ZoomReactingControl {

	private Preset preset = null;
	private RGB color = null;
	private String colorString = null;
	private boolean isVisible = false;
	
	public ColorTag(MapsLayout parent, Preset preset) {
		super(parent);
		this.preset = preset;
		colorString = this.preset.getAttribute("color");
		calcColor();
	}
	
	public void setColor(String col) {
		colorString = col;
		calcColor();
	}
	
	protected void calcColor() {
		//green, blue, yellow, orange, purple, red
		switch(colorString)
		{
			case "green":
				color = new RGB(0, 255, 0);
				break;
			case "blue":
				color = new RGB(0, 0, 255);
				break;
			case "yellow":
				color = new RGB(255, 255, 0);
				break;
			case "orange":
				color = new RGB(255, 126, 0);
				break;
			case "purple":
				color = new RGB(255, 0, 255);
				break;
			case "red":
				color = new RGB(255, 0, 0);
				break;
			default:
				color = new RGB(255, 255, 255);
		}
	}
	
	@Override
	protected double getBasicWidth() {
		return 60;
	}

	@Override
	protected double getBasicHeight() {
		return 15;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}
		
	@Override
	public boolean isVisible() {
		return isVisible && super.isVisible();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (isVisible) {
			Rect pixRect = getPixRect();
			ctx.setFillStyle(color.toString());
			Position center = pixRect.getCenterPoint();
			ctx.fillRect(center.getX(), center.getY(), pixRect.getWidth(), pixRect.getHeight());
		}
	}
	
	@Override
	public double getLevelOfDetailForFullVisibility() {
		return getParent().getLevelOfDetailForFullVisibility() + 1;
	}

}
