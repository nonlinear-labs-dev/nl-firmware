package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Tracer;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class ParameterClippingLabel extends Label {

	String m_text = "";
	
	ParameterClippingLabel(OverlayLayout parent, BeltParameterLayout.Mode mode) {
		super(parent);
		setFontHeightInMM(9);
		setClipping(false);
	}

	public void setClipping(boolean b) {
		Tracer.log("setting clip to " + b);
		m_text = b ? "!" : "";
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return m_text;
	}

}
