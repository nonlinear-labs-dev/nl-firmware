package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SetupLabel extends Label {
	private String label;

	public SetupLabel(OverlayLayout parent, String label) {
		super(parent);
		this.label = label;
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return label;
	}

	@Override
	protected double getFontHeight(Rect pixRect) {
		return super.getFontHeight(pixRect) * 0.80;
	}

	@Override
	protected TextAlign getAlignment() {
		return TextAlign.LEFT;
	}

	@Override
	public RGB getColorFont() {
		return new Gray(229);
	}

	public void setText(String txt) {
		if (!label.equals(txt)) {
			label = txt;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	public String getLabel() {
		return label;
	}

}