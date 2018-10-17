package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class ParameterClippingLabel extends Label {

	boolean m_clipping = false;

	ParameterClippingLabel(OverlayLayout parent, BeltParameterLayout.Mode mode) {
		super(parent);
		setFontHeightInMM(9);
		setClipping(false);
	}

	public void setClipping(boolean b) {
		if (m_clipping != b) {
			m_clipping = b;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return m_clipping ? "!" : "";
	}

}
