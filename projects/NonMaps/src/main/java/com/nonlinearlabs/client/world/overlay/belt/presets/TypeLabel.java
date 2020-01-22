package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.client.world.overlay.Label;

public class TypeLabel extends Label {

	public TypeLabel(BeltPreset parent) {
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
            switch(p.getType())
            {
                case Single:
                    return "";
                case Split:
                    return "\uE0F0";
                case Layer:
                    return "\uE0F3";
            }
		}

		return "";
	}

	protected TextAlign getAlignment() {
		return TextAlign.LEFT;
	}

	@Override
	protected Rect getTextRect() {
		Rect r = super.getTextRect().copy();
		r.applyPadding(0, 0, Millimeter.toPixels(1), 0);
		return r;
	}

}
