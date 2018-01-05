package com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;

public class LittleKnobSlider extends SliderHorizontal {
	public LittleKnobSlider(MapsLayout parent) {
		super(parent);
	}

	@Override
	protected void drawIndicatorArea(Context2d ctx, double centerX, double indicatorAreaWidth, double indicatorStartX) {
	}

	@Override
	protected void drawHandle(Context2d ctx, double indicatorAreaWidth, double indicatorStartX) {
		Rect pixRect = getPixRect();

		ctx.setStrokeStyle(getColorObjectContour().toString());

		Parameter p = getParentParameterLayout();

		if (Math.abs(getValue()) < 0.001 && p.dimHandleAtDefaultValue())
			ctx.setFillStyle(getColorObjectContour().toString());
		else
			ctx.setFillStyle(getColorIndicator().toString());

		double x = indicatorStartX + toXPixels(indicatorAreaWidth);
		ctx.beginPath();
		ctx.arc(x, pixRect.getCenterPoint().getY(), pixRect.getHeight() / 2, 0, 2 * Math.PI);
		ctx.fill();
		ctx.stroke();
	}
}