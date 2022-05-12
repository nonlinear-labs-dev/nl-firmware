package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;

public class LittleKnobSlider extends SliderHorizontal {

	private int paramID;

	public LittleKnobSlider(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		this.paramID = parameterID;
	}

	@Override
	protected void drawIndicatorArea(Context2d ctx, double centerX, double indicatorAreaWidth, double indicatorStartX) {
	}

	protected void drawHandle(Context2d ctx, double indicatorAreaWidth, double indicatorStartX) {
		Rect pixRect = getPixRect();
		ctx.setFillStyle(getColorIndicator().toString());

		double handleWidth = toXPixels(3);
		double width = toXPixels(getBasicWidth() - 41 + 1);

		ctx.fillRect(indicatorStartX + getValue() * width - toXPixels(0.5), pixRect.getTop() + toYPixels(0.5),
				handleWidth, pixRect.getHeight() - toYPixels(1));

		ctx.setFillStyle(getColorObjectContour().toString());
		ctx.fillRect(indicatorStartX + getValue() * width - toXPixels(1) + handleWidth / 2,
				pixRect.getTop() + toYPixels(0.5), toXPixels(1), pixRect.getHeight() - toYPixels(1));

	
		
		if(presenter.modulation.isModulateable)
		{
			drawModIndication(ctx);
		}
	}

	private void drawModIndication(Context2d ctx) {
		boolean isBiPolar = presenter.bipolar;
		double targetValue = presenter.controlPosition;
		double modLeft = presenter.modulation.modulationRange.left;
		double modRight = presenter.modulation.modulationRange.right;

		Rect r = getPixRect().copy();

		double sliderLeftStart = isBiPolar() ? getPixRect().getCenterPoint().getX()
				: getPixRect().getLeft() + toXPixels(19);
		double sliderRightEnd = getPixRect().getRight() - toXPixels(19);

		double width = sliderRightEnd - sliderLeftStart;
		double targetX = 0;

		if (isBiPolar) {
			double x = r.getCenterPoint().getX();
			r.setLeft(x + width * modLeft);
			r.setRight(x + width * modRight);
			targetX = x + width * targetValue;
		} else {
			r.setLeft(sliderLeftStart + width * modLeft);
			r.setRight(sliderLeftStart + width * modRight);
			targetX = sliderLeftStart + width * targetValue;
		}

		r = normalizeRect(r);

		r.moveBy(0, toYPixels(-2.35));
		r.setHeight(r.getHeight() * 0.3);

		RGB fillColor = getColorCorona();

		Rect leftRect = r.copy();
		leftRect.setRight(targetX);
		leftRect.fill(ctx, fillColor);

		Rect rightRect = r.copy();
		double oldWidth = rightRect.getRight() - targetX;

		rightRect.setLeft(targetX);
		rightRect.setWidth(oldWidth);
		rightRect.fill(ctx, fillColor);

		leftRect.moveBy(0, toYPixels(8.9));
		rightRect.moveBy(0, toYPixels(8.9));

		leftRect.fill(ctx, fillColor);
		rightRect.fill(ctx, fillColor);
	}

	protected Rect normalizeRect(Rect r) {
		r.normalize();
		r = r.getIntersection(getPixRect());
		return r;
	}

	protected double getVerticalOffset() {
		return 0;
	}

	protected RGB getColorCorona() {
		return new Gray(200);
	}
}