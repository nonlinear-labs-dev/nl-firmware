package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class KeyTrackParameter extends NumericalControlSmall {
	private boolean isVisible = false;

	public KeyTrackParameter(MapsLayout parent) {
		super(parent);
	}

	@Override
	protected double getMinHeight() {
		return 7.5;
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		isVisible = levelOfDetail >= getLevelOfDetailForFullVisibility();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (isVisible)
			super.draw(ctx, invalidationMask);
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 4;
	}

	@Override
	protected double getBasicHeight() {
		return 25;
	}
}