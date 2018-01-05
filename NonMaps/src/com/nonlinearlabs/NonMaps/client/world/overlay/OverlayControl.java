package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;

public abstract class OverlayControl extends Control {

	private Rect relativePosition = new Rect();

	public OverlayControl(Control parent) {
		super(parent);
	}

	public Rect getRelativePosition() {
		return relativePosition;
	}

	public void setRelativePosition(Rect relativePosition) {
		this.relativePosition = relativePosition;
		this.relativePosition.getDimension().makeNonNegative();
	}

	public void doLayout(double x, double y, double w, double h) {
		getRelativePosition().getDimension().set(w, h);
		getRelativePosition().getPosition().setX(x);
		getRelativePosition().getPosition().setY(y);
	}

	public Rect getPositionRelativeToParent(OverlayControl parent) {
		Rect r = relativePosition.copy();
		OverlayControl p = (OverlayControl) getParent();

		while (p != parent) {
			r.moveBy(p.getRelativePosition().getPosition());
			p = (OverlayControl) p.getParent();
		}

		return r;
	}

	@Override
	public void calcPixRect(Position parentsReference, double currentZoom) {
		Rect newPos = getRelativePosition().copy();
		newPos.getPosition().moveBy(parentsReference);
		setPixRect(newPos);
	}

	@Override
	public abstract void draw(Context2d ctx, int invalidationMask);

	void center() {
		Rect myPos = getRelativePosition();
		Rect parentPos = ((OverlayControl) getParent()).getRelativePosition();

		myPos.setLeft((parentPos.getWidth() - myPos.getWidth()) / 2);
		myPos.setTop((parentPos.getHeight() - myPos.getHeight()) / 2);
	}

	static public double getButtonDimension() {
		return SVGImage.calcSVGDimensionToPixels(28);
	}

	static public double getSpaceBetweenChildren() {
		return SVGImage.calcSVGDimensionToPixels(1);
	}
}
