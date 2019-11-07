package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

abstract class Knob extends ModulateableValueControl {

	private static double startAngle = degreeToRadians(120);
	private static double endAngle = degreeToRadians(60);

	public Knob(MapsLayout parent, int parameterId) {
		super(parent, parameterId);
	}

	private static double degreeToRadians(double degree) {
		return 2 * Math.PI * degree / 360;
	}

	@Override
	protected void drawCenterIndicator(Context2d ctx) {
	}

	@Override
	protected void drawBackgroundAndOutline(Context2d ctx) {
		Position center = getKnobRect().getCenterPoint();
		ctx.beginPath();
		ctx.setFillStyle(getColorSliderBackground().toString());
		ctx.setStrokeStyle(getColorObjectContour().toString());
		ctx.setLineWidth(toXPixels(0.5));
		ctx.arc(center.getX(), center.getY(), toXPixels(getOuterRadius()), startAngle, endAngle, false);
		ctx.arc(center.getX(), center.getY(), toXPixels(getInnerRadius()), endAngle, startAngle, true);
		ctx.closePath();
		ctx.fill();
		ctx.stroke();
	}

	@Override
	protected void drawActiveArea(Context2d ctx) {
		Rect r = getKnobRect();
		Position center = r.getCenterPoint();

		double endAngleValue = 0;
		double startAngleValue = 0;

		double value = getValue();

		if (isBiPolar()) {
			startAngleValue = degreeToRadians(270);
			endAngleValue = degreeToRadians(270 + value * 150);
		} else {
			startAngleValue = startAngle;
			endAngleValue = degreeToRadians(120 + value * 300);
		}

		drawHighlitArea(ctx, center, endAngleValue, startAngleValue, value);
		drawIndicator(ctx, center, endAngleValue, value);

		ctx.beginPath();

		if (Math.abs(getValue()) < 0.001)
			ctx.setFillStyle(getColorSliderBackground().toString());
		else
			ctx.setFillStyle(getColorSliderHighlight().toString());

		ctx.setStrokeStyle(getColorObjectContour().toString());
		ctx.setLineWidth(toXPixels(0.5));
		ctx.arc(center.getX(), center.getY(), toXPixels(getCenterPointRadius()), 0, 2 * Math.PI, false);
		ctx.closePath();

		ctx.fill();
		ctx.stroke();
	}

	protected void drawHighlitArea(Context2d ctx, Position center, double endAngleValue, double startAngleValue,
			double value) {
		boolean arcDirection = value < 0;
		ctx.beginPath();
		ctx.setStrokeStyle(getColorObjectContour().toString());
		ctx.setLineWidth(toXPixels(0.5));
		ctx.setFillStyle(getColorSliderHighlight().toString());
		ctx.arc(center.getX(), center.getY(), toXPixels(getOuterRadius()), startAngleValue, endAngleValue,
				arcDirection);
		ctx.arc(center.getX(), center.getY(), toXPixels(getInnerRadius()), endAngleValue, startAngleValue,
				!arcDirection);
		ctx.closePath();
		ctx.fill();
		ctx.stroke();
	}

	@Override
	protected void drawCorona(Context2d ctx, double modLeft, double modRight) {

		Position center = getKnobRect().getCenterPoint();

		double startAngleValue = 0;
		double endAngleValue = 0;

		if (isBiPolar()) {
			startAngleValue = degreeToRadians(270 + modLeft * 150);
			endAngleValue = degreeToRadians(270 + modRight * 150);
		} else {
			startAngleValue = degreeToRadians(120 + modLeft * 300);
			endAngleValue = degreeToRadians(120 + modRight * 300);
		}

		ctx.setStrokeStyle(getColorCorona().toString());

		ctx.setLineWidth(toXPixels(2));

		ctx.beginPath();
		ctx.arc(center.getX(), center.getY(), toXPixels(getOuterRadius() + 1.5), startAngleValue, endAngleValue, false);
		ctx.stroke();

		ctx.beginPath();
		ctx.arc(center.getX(), center.getY(), toXPixels(getInnerRadius() - 1.5), startAngleValue, endAngleValue, false);
		ctx.stroke();
	}

	private Rect getKnobRect() {
		Rect pixRect = getPixRect().copy();
		pixRect.moveBy(0, toYPixels(getVerticalOffsetInUnits()));
		return pixRect;
	}

	protected void drawIndicator(Context2d ctx, Position center, double endAngleValue, double value) {

		double endX = center.getX() + Math.cos(endAngleValue) * toXPixels(getOuterRadius() - 0.25);
		double endY = center.getY() + Math.sin(endAngleValue) * toYPixels(getOuterRadius() - 0.25);

		if (Math.abs(value) < 0.001) {
			ctx.setStrokeStyle(getColorObjectContour().toString());
		} else {
			ctx.beginPath();
			ctx.setStrokeStyle(getColorObjectContour().toString());
			ctx.setLineWidth(toXPixels(2.5));
			ctx.moveTo(center.getX(), center.getY());
			ctx.lineTo(endX, endY);
			ctx.stroke();

			ctx.setStrokeStyle(getColorIndicator().toString());
		}

		ctx.beginPath();
		ctx.setLineWidth(toXPixels(1.5));
		ctx.moveTo(center.getX(), center.getY());
		ctx.lineTo(endX, endY);
		ctx.stroke();
	}

	@Override
	protected double getBasicWidth() {
		return 70;
	}

	@Override
	protected double getBasicHeight() {
		return 45;
	}

	protected double getCenterPointRadius() {
		return 2.5;
	}

	protected double getInnerRadius() {
		return 16;
	}

	protected double getOuterRadius() {
		return 21;
	}

	protected double getVerticalOffsetInUnits() {
		return 0;
	}

	@Override
	public double getMaxLevelOfDetail() {
		return 6;
	}
}
