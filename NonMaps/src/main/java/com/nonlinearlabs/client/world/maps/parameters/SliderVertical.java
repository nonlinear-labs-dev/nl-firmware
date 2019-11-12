package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class SliderVertical extends ModulateableValueControl {

	public SliderVertical(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	@Override
	protected void drawActiveArea(Context2d ctx) {
		Rect r = getSliderRect();
		r.normalize();

		double handleHeight = getHandleHeight();
		double handleHeightPixels = toYPixels(handleHeight);

		r.reduceHeightBy(handleHeightPixels);
		r.reduceWidthBy(toXPixels(1));

		Position center = r.getCenterPoint();
		double value = adaptValue(getValue());

		if (isBiPolar()) {
			r.setTop(center.getY());
			r.setHeight(-1 * value * r.getHeight() / 2);
		} else {
			r.setTop(r.getBottom());
			r.setHeight(-1 * r.getHeight() * value);
		}

		r.fill(ctx, getColorSliderHighlight());

		RGB handleColor = (Math.abs(adaptValue(getValue())) < 0.001) ? getColorInactiveHandle() : getColorIndicator();

		r.moveTo(r.getLeft(), r.getBottom() - handleHeightPixels / 2);
		r.setHeight(handleHeightPixels);
		r.fill(ctx, handleColor);
	}

	protected double getHandleHeight() {
		return 1;
	}

	protected RGB getColorInactiveHandle() {
		return getColorObjectContour();
	}

	@Override
	protected void drawBackgroundAndOutline(Context2d ctx) {
		getSliderRect().fillAndStroke(ctx, getColorSliderBackground(), toXPixels(1), getColorObjectContour());
	}

	@Override
	protected void drawCenterIndicator(Context2d ctx) {
		if (isBiPolar()) {
			Rect r = getSliderRect();
			Position center = r.getCenterPoint();
			double handleWidth = toXPixels(getSliderHandleWidth());
			double handleHeight = toYPixels(0.5);
			Rect centerRect = new Rect(center.getX() - handleWidth, center.getY() - handleHeight, handleWidth * 2,
					handleHeight * 2);
			centerRect.fill(ctx, getColorSliderBackground());
		}
	}

	@Override
	protected void drawCorona(Context2d ctx, double modLeft, double modRight) {
		Rect r = getSliderRect();

		double startAngleValue = 0;
		double endAngleValue = 0;

		if (isBiPolar()) {
			startAngleValue = r.getCenterPoint().getY() - modLeft * r.getHeight() / 2;
			endAngleValue = r.getCenterPoint().getY() - modRight * r.getHeight() / 2;
		} else {
			startAngleValue = r.getBottom() - modLeft * r.getHeight();
			endAngleValue = r.getBottom() - modRight * r.getHeight();
		}

		r.setTop(startAngleValue);
		r.setHeight(endAngleValue - startAngleValue);
		r.setWidth(toXPixels(2));

		r.moveBy(toXPixels(-2.5), 0);
		r.fill(ctx, getColorCorona());

		r.moveBy(getSliderDimension().getWidth() + toXPixels(3), 0);
		r.fill(ctx, getColorCorona());
	}

	protected Rect getSliderRect() {
		Rect r = new Rect(getPixRect().getPosition(), getSliderDimension());
		r.centerIn(getPixRect());
		r.moveBy(0, toYPixels(getVerticalOffset()));
		return r;
	}

	private Dimension getSliderDimension() {
		if (shouldDrawThicker())
			return new Dimension(toXPixels(10), toYPixels(getBasicHeight() - 4));

		return new Dimension(toXPixels(8), toYPixels(getBasicHeight() - 4));
	}

	protected double getSliderHandleWidth() {
		if (shouldDrawThicker())
			return 10;

		return 7;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	@Override
	protected double getBasicHeight() {
		return 80;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	private double getVerticalOffset() {
		return 0;
	}

	@Override
	public double getMaxLevelOfDetail() {
		return 6;
	}
}
