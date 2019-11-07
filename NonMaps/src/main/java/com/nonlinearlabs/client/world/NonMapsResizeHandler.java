package com.nonlinearlabs.client.world;

import com.google.gwt.event.logical.shared.ResizeEvent;
import com.google.gwt.event.logical.shared.ResizeHandler;

class NonMapsResizeHandler implements ResizeHandler {

	private final Mouseing nonMaps;

	/**
	 * @param mouseAwareEntryPoint
	 */
	NonMapsResizeHandler(Mouseing mouseAwareEntryPoint) {
		this.nonMaps = mouseAwareEntryPoint;
	}

	@Override
	public void onResize(ResizeEvent event) {
		this.nonMaps.invalidate(Control.INVALIDATION_FLAG_ZOOMED);
	}
}