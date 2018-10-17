package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class ABSlider extends SliderVertical {
	public ABSlider(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		Rect pixRect = getPixRect();
		Rect sliderRect = getSliderRect();

		ctx.beginPath();
		ctx.setStrokeStyle(RGB.black().toString());

		double handleHeight = toYPixels(getHandleHeight());
		double value = sliderRect.getTop() + handleHeight / 2 + getValue() * (sliderRect.getHeight() - handleHeight);
		ctx.moveTo(sliderRect.getLeft(), value);
		ctx.lineTo(sliderRect.getRight(), value);
		ctx.stroke();

		double fontHeightInUnits = getFontHeight();
		double fontHeightInPixels = toYPixels(fontHeightInUnits);

		ctx.setTextAlign(TextAlign.CENTER);
		ctx.setFillStyle(getColorFont().toString());
		ctx.setFont(fontHeightInPixels + "px " + getFontName());

		double x = sliderRect.getLeft() - fontHeightInPixels;
		ctx.setTextBaseline(TextBaseline.TOP);
		ctx.fillText("A", x, pixRect.getTop());

		ctx.setTextBaseline(TextBaseline.BOTTOM);
		ctx.fillText("B", x, pixRect.getBottom());
	}

	@Override
	public RGB getColorSliderHighlight() {
		return super.getColorSliderBackground();
	}

	@Override
	public double adaptValue(double v) {
		return 1 - v;
	}

	@Override
	protected RGB getColorInactiveHandle() {
		return super.getColorIndicator();
	}

	@Override
	protected double getHandleHeight() {
		return super.getHandleHeight() * 5;
	}
}