package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableSliderHorizontal extends ModulateableValueControl {

	public ModulateableSliderHorizontal(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

    @Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (!isVisible())
			return;

		Rect pixRect = getPixRect();

		double centerX = pixRect.getCenterPoint().getX();

		if (isBiPolar()) {
			Rect middle = new Rect(centerX - toXPixels(0.5), pixRect.getTop() - toYPixels(4), toXPixels(1),
					pixRect.getHeight() + toYPixels(8));
			middle.fill(ctx, getColorObjectContour());
		}

		double indicatorAreaWidth = getIndicatorAreaWidth();
		double indicatorStartX = isBiPolar() ? centerX : pixRect.getLeft() + toXPixels(19);

		drawBackground(ctx);
		drawIndicatorArea(ctx, centerX, indicatorAreaWidth, indicatorStartX);
		drawHandle(ctx, indicatorAreaWidth, indicatorStartX);
	}

	protected void drawBackground(Context2d ctx) {
		Rect background = calcBackgroundRect();
		background.fillAndStroke(ctx, getColorSliderBackground(), toXPixels(1), getColorObjectContour());
	}

	protected Rect calcBackgroundRect() {
		Rect background = getPixRect().copy();
		double xPadding = toXPixels(18.5);
		double yPadding = 0;
		background.applyPadding(xPadding, yPadding, xPadding, yPadding);
		return background;
	}

	protected double getIndicatorAreaWidth() {
		double indicatorAreaWidth = (getBasicWidth() - 38) * getValue();

		if (isBiPolar())
			indicatorAreaWidth = indicatorAreaWidth / 2;

		return indicatorAreaWidth;
	}

	protected void drawIndicatorArea(Context2d ctx, double centerX, double indicatorAreaWidth, double indicatorStartX) {
		Rect pixRect = getPixRect();
		ctx.setFillStyle(getColorSliderHighlight().toString());
		ctx.fillRect(indicatorStartX, pixRect.getTop() + toYPixels(0.5), toXPixels(indicatorAreaWidth),
				pixRect.getHeight() - toYPixels(1));
	}

	protected void drawHandle(Context2d ctx, double indicatorAreaWidth, double indicatorStartX) {
		Rect pixRect = getPixRect();

		if (Math.abs(getValue()) < 0.001)
			ctx.setFillStyle(getColorObjectContour().toString());
		else {
			ctx.setFillStyle(getColorObjectContour().toString());
			ctx.fillRect(indicatorStartX + toXPixels(indicatorAreaWidth - 1), pixRect.getTop() + toYPixels(0.5),
					toXPixels(2), pixRect.getHeight() - toYPixels(1));
			ctx.setFillStyle(getColorIndicator().toString());
		}

		ctx.fillRect(indicatorStartX + toXPixels(indicatorAreaWidth - 0.5), pixRect.getTop() + toYPixels(0.5),
				toXPixels(1), pixRect.getHeight() - toYPixels(1));
	}

    @Override
    protected void drawCenterIndicator(Context2d ctx) {
        // TODO Auto-generated method stub

    }

    @Override
    protected void drawBackgroundAndOutline(Context2d ctx) {
        // TODO Auto-generated method stub

    }

    @Override
    protected void drawActiveArea(Context2d ctx) {
        // TODO Auto-generated method stub

    }

    @Override
    protected void drawCorona(Context2d ctx, double left, double right) {
        // TODO Auto-generated method stub

    }

    @Override
    protected double getBasicWidth() {
        return 80;
    }

    @Override
    public double getTopMargin() {
        return 4;
    }

    @Override
    protected double getBasicHeight() {
        return 6;
    }

    @Override
    protected double getMinHeight() {
        return 5;
    }
}
