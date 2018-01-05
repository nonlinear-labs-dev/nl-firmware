package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.Label;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

class SetupHeader extends Label {
	SetupHeader(MapsLayout parent) {
		super(parent, "Developer Options");
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().fill(ctx, getParent().getColorFont());
		super.draw(ctx, invalidationMask);
	}

	@Override
	protected double getFontHeight() {
		return super.getFontHeight() * 1.25;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 0;
	}

	@Override
	public RGB getColorFont() {
		return new Gray(0);
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
		setNonSize(parentsWidthFromFirstPass, getNonPosition().getHeight());
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		return this;
	}

	@Override
	public Control startDragging(Position pos) {
		return getNonMaps().getNonLinearWorld().getViewport().getOverlay().createDragProxy(getParent(), getPixRect().getPosition());
	}

	@Override
	public Control click(Position eventPoint) {
		return this;
	}
}