package com.nonlinearlabs.client.world.overlay;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.html.overlayControlDialog.OverlayControlDialog;

public class ContextMenu extends OverlayLayout {

	private OverlayControlDialog dialog = new OverlayControlDialog(this, "overlay-control-dialog");

	public ContextMenu(OverlayLayout parent) {
		super(parent);
		dialog.show();
	}

	@Override
	public void setPixRect(Rect rect) {
		super.setPixRect(rect);
		dialog.sync(this);
	}

	@Override
	public void movePixRect(double x, double y) {
		super.movePixRect(x, y);
		dialog.sync(this);
	}

	@Override
	public OverlayLayout getParent() {
		return (OverlayLayout) super.getParent();
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		hide();
		return super.mouseDown(eventPoint);
	}

	@Override
	public void onRemoved() {
		super.onRemoved();
		dialog.hide();
	}

	@Override
	public void draw(Context2d c, int invalidationMask) {
		Context2d ctx = dialog.getContext();

		Rect r = getPixRect();

		ctx.save();
		ctx.translate(-r.getLeft(), -r.getTop());

		r.drawRoundedArea(ctx, Millimeter.toPixels(1), Millimeter.toPixels(0.7), RGB.black(), RGB.black());
		r.drawRoundedArea(ctx, Millimeter.toPixels(1), Millimeter.toPixels(0.25), new Gray(77), new Gray(128));
		super.draw(ctx, invalidationMask);
		ctx.restore();
	}

	public void hide() {
		getParent().removeChild(this);
		getParent().requestLayout();
		invalidate(INVALIDATION_FLAG_ANIMATION_PROGRESS);
	}

	public double getDesiredWidth() {
		double maxWidth = 0;

		for (OverlayControl c : getChildren()) {
			if (c instanceof ContextMenuItem) {
				ContextMenuItem i = (ContextMenuItem) c;
				maxWidth = Math.max(maxWidth, i.getDesiredWidth());
			}
		}

		return maxWidth + 2 * getXMargin();
	}

	public double getDesiredHeight() {
		double maxHeight = 0;

		for (OverlayControl c : getChildren()) {
			if (c instanceof ContextMenuItem) {
				ContextMenuItem i = (ContextMenuItem) c;
				maxHeight += i.getDesiredHeight();
			}
		}

		return maxHeight;
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double y1 = 0;
		double x1 = getXMargin();

		for (OverlayControl c : getChildren()) {
			ContextMenuItem i = (ContextMenuItem) c;
			double h1 = i.getDesiredHeight();
			c.doLayout(x1, y1, w - 2 * x1, h1);
			y1 += h1;
		}
	}

	private double getXMargin() {
		return Millimeter.toPixels(2.5);
	}
}
