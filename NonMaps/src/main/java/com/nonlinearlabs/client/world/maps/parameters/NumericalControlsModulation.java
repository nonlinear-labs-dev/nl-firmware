package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.contextStates.ClipContext;
import com.nonlinearlabs.client.contextStates.ContextState;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

class NumericalControlsModulation extends ModulateableValueControl {

	NumericalControlsModulation(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 15;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	private Rect getModulationStuffRect() {
		Rect r = getPixRect().copy();
		r.reduceWidthBy(toXPixels(10));
		return r;
	}

	@Override
	protected void drawCenterIndicator(Context2d ctx) {

	}

	@Override
	protected void drawBackgroundAndOutline(Context2d ctx) {
		Rect sliderRect = getModulationStuffRect();
		sliderRect.fillAndStroke(ctx, getColorSliderBackground(), toXPixels(1), getColorObjectContour());

		double modWidth = getModWidth();
		ctx.beginPath();
		ctx.moveTo(sliderRect.getLeft(), sliderRect.getTop() + modWidth);
		ctx.lineTo(sliderRect.getRight(), sliderRect.getTop() + modWidth);
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(sliderRect.getLeft(), sliderRect.getBottom() - modWidth);
		ctx.lineTo(sliderRect.getRight(), sliderRect.getBottom() - modWidth);
		ctx.stroke();
	}

	private double getModWidth() {
		return toYPixels(2.5);
	}

	@Override
	protected void drawActiveArea(Context2d ctx) {
		Rect sliderRect = getModulationStuffRect();

		try (ContextState s = new ClipContext(ctx, sliderRect.getReducedBy(getConturPixels()))) {
			sliderRect.moveBy(0, toYPixels(2.5));
			sliderRect.setHeight(sliderRect.getHeight() - 2 * getModWidth());
			sliderRect.reduceWidthBy(getConturPixels());
			sliderRect.reduceHeightBy(getConturPixels());

			Rect indicator = sliderRect.copy();

			double value = getValue();
			indicator.setWidth(sliderRect.getWidth() * value);
			indicator.fill(ctx, getColorSliderHighlight());

			indicator.setLeft(indicator.getRight() - toXPixels(0.5));
			indicator.setWidth(toXPixels(1));
			indicator.fill(ctx, getColorIndicator());
		}
	}

	@Override
	protected void drawCorona(Context2d ctx, double left, double right) {
		Rect sliderRect = getModulationStuffRect();
		sliderRect.setHeight(getModWidth());
		sliderRect.reduceWidthBy(getConturPixels() * 1);
		sliderRect.reduceHeightBy(getConturPixels());

		Rect indicator = sliderRect.copy();

		if (isBiPolar()) {
			left += 1;
			left /= 2;

			right += 1;
			right /= 2;
		}

		indicator.setWidth(sliderRect.getWidth() * (right - left));
		indicator.setLeft(sliderRect.getLeft() + sliderRect.getWidth() * left);
		indicator.fill(ctx, getColorCorona());

		indicator.moveTo(indicator.getLeft(), getModulationStuffRect().getBottom() - getModWidth() + toYPixels(0.5));
		indicator.fill(ctx, getColorCorona());
	}
}
