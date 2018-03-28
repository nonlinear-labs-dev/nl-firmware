package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.LineCap;
import com.google.gwt.canvas.dom.client.Context2d.LineJoin;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.FloatingWindow;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class FloatingWindowHeader extends OverlayLayout {

	protected Label header;
	protected Label x;

	public FloatingWindowHeader(final FloatingWindow parent) {
		super(parent);

		addChild(header = new Label(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return parent.getTitle();
			}
		});

		addChild(x = new Label(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return "";
			}

			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				drawX(ctx);
			}

			private void drawX(Context2d ctx) {
				Rect r = getPixRect().getReducedBy(Millimeter.toPixels(4));
				r.drawRoundedRect(ctx, Rect.ROUNDING_ALL, Millimeter.toPixels(1), Millimeter.toPixels(0.25), new Gray(54), new Gray(84));

				r = r.getReducedBy(Millimeter.toPixels(4));

				ctx.beginPath();
				ctx.moveTo(r.getLeft(), r.getTop());
				ctx.lineTo(r.getRight(), r.getBottom());
				ctx.moveTo(r.getRight(), r.getTop());
				ctx.lineTo(r.getLeft(), r.getBottom());

				ctx.setStrokeStyle(new Gray(120).toString());
				ctx.setLineWidth(Millimeter.toPixels(0.5));
				ctx.setLineCap(LineCap.SQUARE);
				ctx.setLineJoin(LineJoin.MITER);

				ctx.stroke();
			}

			@Override
			public Control mouseDown(Position eventPoint) {
				return this;
			}

			@Override
			public Control click(Position eventPoint) {
				parent.toggle();
				return this;
			}
		});
	}

	@Override
	public FloatingWindow getParent() {
		return (FloatingWindow) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double buttonDim = getButtonDimension();
		this.x.doLayout(w - buttonDim, (h - buttonDim) / 2, buttonDim, buttonDim);
		this.header.doLayout(buttonDim, 0, w - 2 * buttonDim, h);
	}

	protected void toggle() {
		getParent().toggle();
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
		return this;
	}
		
	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		getParent().getRelativePosition().moveBy(newPoint.getX() - oldPoint.getX(), newPoint.getY() - oldPoint.getY());
		preventWindowOutsideOfCanvasTopAndBottom();
		preventWindowOutsideOfLeftAndRight();
		requestLayout();
		return this;
	}

	public void preventWindowOutsideOfCanvasTopAndBottom() {
		double minY = 0;
		double maxY = NonMaps.theMaps.getCanvas().getCoordinateSpaceHeight() - getPixRect().getHeight();

		Rect relPosition = getParent().getRelativePosition();

		if (relPosition.getTop() < minY)
			relPosition.moveTo(relPosition.getLeft(), minY);

		if (relPosition.getTop() > maxY)
			relPosition.moveTo(relPosition.getLeft(), maxY);
	}
	
	public void preventWindowOutsideOfLeftAndRight() {
		double minX = 0;
		double maxX = NonMaps.theMaps.getCanvas().getCoordinateSpaceWidth() - getPixRect().getWidth();
		Rect relPosition = getParent().getRelativePosition();
		double left = relPosition.getLeft() ;
		
		if (left < (minX - (relPosition.getWidth() / 2)))
			relPosition.moveTo(minX - relPosition.getWidth() / 2, relPosition.getTop());

		if ((left - (relPosition.getWidth() / 2)) > maxX)
			relPosition.moveTo(maxX + relPosition.getWidth() / 2, relPosition.getTop());
	}

	@Override
	public Control click(Position eventPoint) {
		return this;
	}

}