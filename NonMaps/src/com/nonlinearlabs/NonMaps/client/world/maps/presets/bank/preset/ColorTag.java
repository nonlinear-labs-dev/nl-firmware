package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ZoomReactingControl;

public class ColorTag extends ZoomReactingControl {

	public enum Color {
		green, blue, yellow, orange, purple, red, none;

		public RGB toRGB() {
			switch (this) {
			case green:
				return new RGB(0, 255, 0);
			case blue:
				return new RGB(0, 0, 255);
			case yellow:
				return new RGB(255, 255, 0);
			case orange:
				return new RGB(255, 126, 0);
			case purple:
				return new RGB(255, 0, 255);
			case red:
				return new RGB(255, 0, 0);
			case none:
				return new RGBA(0, 0, 0, 0);

			default:
				return null;
			}
		}
	}

	public ColorTag(MapsLayout parent) {
		super(parent);
	}

	protected RGB calcColor() {
		try {

			return Color.valueOf(getParent().getAttribute("color")).toRGB();
		} catch (Exception e) {
			return null;
		}
	}

	@Override
	public Preset getParent() {
		return (Preset) super.getParent();
	}

	@Override
	protected double getBasicWidth() {
		return 6;
	}

	@Override
	protected double getBasicHeight() {
		return 30;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		RGB color = calcColor();
		if (color == null)
			return;

		Rect r = getPixRect().copy();
		r.reduceHeightBy(r.getHeight() / 3);
		r.fill(ctx, color);
	}
}
