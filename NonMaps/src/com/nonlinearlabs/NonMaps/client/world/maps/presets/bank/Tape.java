package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Dimension;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.overlay.DragProxy;

public class Tape extends MapsControl {

	public enum Orientation {
		North, South, East, West
	}

	private Orientation orientation;

	public Tape(Bank parent, Orientation orientation) {
		super(parent);
		this.setOrientation(orientation);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
	}

	@Override
	public Bank getParent() {
		return (Bank) super.getParent();
	}

	@Override
	public boolean isVisible() {
		boolean v = super.isVisible();
		v &= getNonMaps().getNonLinearWorld().getViewport().getOverlay().isCurrentlyDraggingATypeOf(Bank.class.getName());
		v &= getParent().isTapeActive(orientation);
		return v;
	}

	public Orientation getOrientation() {
		return orientation;
	}

	public void setOrientation(Orientation orientation) {
		this.orientation = orientation;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		getPixRect().fill(ctx, getParent().getParent().isAttachingTape(this) ? new RGB(255, 0, 0) : new RGB(0, 255, 0));
	}

	public boolean fitsTo(Tape others) {
		return (orientation == Orientation.East && others.orientation == Orientation.West)
				|| (orientation == Orientation.West && others.orientation == Orientation.East)
				|| (orientation == Orientation.North && others.orientation == Orientation.South)
				|| (orientation == Orientation.South && others.orientation == Orientation.North);
	}

	@Override
	public Control drag(Position pos, DragProxy dragProxy) {
		if (dragProxy.getOrigin() instanceof Bank) {
			Bank other = (Bank) dragProxy.getOrigin();

			if (getParent() != other) {
				Dimension offset = dragProxy.getPixRect().getLeftTop().getVector(other.getPixRect().getLeftTop());

				for (Tape others : other.getTapes()) {
					if (fitsTo(others)) {
						if (getPixRect().intersects(others.getPixRect().getMovedBy(offset))) {
							getParent().getParent().setAttachingTapes(this, others);
							return this;
						}
					}
				}
			}
		}
		return super.drag(pos, dragProxy);
	}

	@Override
	public Control drop(Position pos, DragProxy dragProxy) {
		if (dragProxy.getOrigin() instanceof Bank) {
			Bank other = (Bank) dragProxy.getOrigin();
			Position pixPos = dragProxy.getPixRect().getPosition();
			NonPosition nonPos = NonMaps.get().getNonLinearWorld().toNonPosition(pixPos);
			NonMaps.get().getServerProxy().dockBanks(getParent(), orientation, other, nonPos);
			return this;
		}
		return super.drop(pos, dragProxy);
	}
}
