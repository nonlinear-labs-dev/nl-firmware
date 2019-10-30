package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.useCases.IncrementalChanger;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

class ValueEdit extends Label {

	private IncrementalChanger changer;
	private BasicParameterModel param;

	ValueEdit(OverlayLayout parent, BasicParameterModel param) {
		super(parent);
		this.param = param;

		param.value.value.onChange(i -> {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return true;
		});
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return param.value.getDecoratedValue(true, true);
	}

	@Override
	public Control click(Position eventPoint) {
		Rect leftRect = getPixRect().copy();
		leftRect.setRight(getPixRect().getLeft() + getPixRect().getWidth() / 2);
		Rect rightRect = getPixRect().copy();
		rightRect.setLeft(getPixRect().getRight() - getPixRect().getWidth() / 2);
		if (leftRect.contains(eventPoint)) {
			EditBufferUseCases.get().decParameter(param.id, false);
			return this;
		} else if (rightRect.contains(eventPoint)) {
			EditBufferUseCases.get().incParameter(param.id, false);
			return this;
		}
		return super.click(eventPoint);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		changer = EditBufferUseCases.get().startUserEdit(param.id, Millimeter.toPixels(100));
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		changer = null;
		return this;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		double xPix = newPoint.getX() - oldPoint.getX();
		double yPix = oldPoint.getY() - newPoint.getY();
		double pix = xPix;

		if (Math.abs(yPix) > Math.abs(xPix))
			pix = yPix;

		if (changer != null)
			changer.changeBy(fine, pix);

		return this;
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		if (amount > 0)
			EditBufferUseCases.get().incParameter(param.id, fine);
		else if (amount < 0)
			EditBufferUseCases.get().incParameter(param.id, fine);

		return this;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		boolean withArrows = getPixRect().getWidth() >= Millimeter.toPixels(50);
		getPixRect().drawValueEditSliderBackgound(ctx, withArrows, getColorFont());
		super.draw(ctx, invalidationMask);
	}

	@Override
	protected Rect getTextRect() {
		Rect r = super.getTextRect().copy();
		r.reduceWidthBy(Millimeter.toPixels(5));
		return r;
	}
}