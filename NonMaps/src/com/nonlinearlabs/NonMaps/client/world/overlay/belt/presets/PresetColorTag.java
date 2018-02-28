package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class PresetColorTag extends OverlayControl {

	public PresetColorTag(BeltPreset parent) {
		super(parent);
	}

	@Override
	public BeltPreset getParent() {
		return (BeltPreset) super.getParent();
	}

	protected RGB calcColor() {

		switch (getParent().getMapsPreset().getAttribute("color")) {
		case "green":
			return new RGB(0, 255, 0);
		case "blue":
			return new RGB(0, 0, 255);
		case "yellow":
			return new RGB(255, 255, 0);
		case "orange":
			return new RGB(255, 126, 0);
		case "purple":
			return new RGB(255, 0, 255);
		case "red":
			return new RGB(255, 0, 0);
		default:
			return null;
		}
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		RGB color = calcColor();
		if (color == null)
			return;
		Rect pixRect = getPixRect().copy();
		double padding = getRelativePosition().getHeight() / 10;
		pixRect.applyPadding(0, padding, 0, padding);
		pixRect.fill(ctx, color);
	}
}
