package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.QuantizedClippedValue;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.QuantizedClippedValue.IncrementalChanger;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

abstract class ValueEdit extends Label implements QuantizedClippedValue.ChangeListener {
	QuantizedClippedValue value = new QuantizedClippedValue(this);
	private IncrementalChanger changer;

	ValueEdit(OverlayLayout parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return getDecoratedValue(true);
	}

	public String getDecoratedValue(boolean withUnit) {
		return getDecoratedValue(withUnit, value.getQuantizedClipped());
	}

	public abstract String getDecoratedValue(boolean withUnit, double cpValue);

	@Override
	public Control click(Position eventPoint) {
		Rect leftRect = getPixRect().copy();
		leftRect.setRight(getPixRect().getLeft() + getPixRect().getWidth() / 2);
		Rect rightRect = getPixRect().copy();
		rightRect.setLeft(getPixRect().getRight() - getPixRect().getWidth() / 2);
		if (leftRect.contains(eventPoint)) {
			value.dec(Initiator.EXPLICIT_USER_ACTION, false);
			return this;
		} else if (rightRect.contains(eventPoint)) {
			value.inc(Initiator.EXPLICIT_USER_ACTION, false);
			return this;
		}
		return super.click(eventPoint);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		changer = value.startUserEdit(Millimeter.toPixels(100));
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
			value.inc(Initiator.EXPLICIT_USER_ACTION, fine);
		else if (amount < 0)
			value.dec(Initiator.EXPLICIT_USER_ACTION, fine);

		return this;
	}

	@Override
	public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
	}

	@Override
	public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
	}

	@Override
	public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue) {
		if (initiator == Initiator.EXPLICIT_USER_ACTION)
			NonMaps.theMaps.getServerProxy().setRandomAmount(newQuantizedValue);
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
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