package com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;

public class LittleKnobSlider extends SliderHorizontal {
	public LittleKnobSlider(MapsLayout parent) {
		super(parent);
	}

	@Override
	protected void drawIndicatorArea(Context2d ctx, double centerX, double indicatorAreaWidth, double indicatorStartX) {
	}

	protected void drawHandle(Context2d ctx, double indicatorAreaWidth, double indicatorStartX) {
		Rect pixRect = getPixRect();

		ctx.setFillStyle(getColorIndicator().toString());

		double handleWidth = toXPixels(3);
		double width = toXPixels(getBasicWidth() - 41 + 1) - handleWidth;

		ctx.fillRect(indicatorStartX + getValue() * width - toXPixels(0.5), pixRect.getTop() + toYPixels(0.5),
				handleWidth, pixRect.getHeight() - toYPixels(1));

		ctx.setFillStyle(getColorObjectContour().toString());
		ctx.fillRect(indicatorStartX + getValue() * width - toXPixels(1) + handleWidth / 2,
				pixRect.getTop() + toYPixels(0.5), toXPixels(1), pixRect.getHeight() - toYPixels(1));

	}

}