package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Dimension;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.NonDimension;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.overlay.DragProxy;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public class Tape extends MapsControl {

	public enum Orientation {
		North, South, East, West
	}

	private Orientation orientation;
	private Bank prospectBank;

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
		return (isActiveEmptyTape() || isActiveInsertTape());
	}

	private boolean isActiveEmptyTape() {
		Overlay o = getNonMaps().getNonLinearWorld().getViewport().getOverlay();

		for (DragProxy d : o.getDragProxies()) {
			Control r = d.getCurrentReceiver();
			if (r != null) {

				if (r instanceof PresetManager || r instanceof Tape) {
					boolean visible = super.isVisible();
					return visible && o.isCurrentlyDraggingATypeOf(Bank.class.getName())
							&& getParent().isTapeActive(orientation);
				}
			}
		}

		return false;
	}

	private boolean isInsertTape() {
		return getParent().hasSlaveInDirection(getOrientation());
	}

	private boolean isCurrentlyDraggedOverMe() {
		if (!isInsertTape())
			return false;

		Overlay o = getNonMaps().getNonLinearWorld().getViewport().getOverlay();
		for (DragProxy d : o.getDragProxies()) {
			Control r = d.getCurrentReceiver();
			if (r == this)
				if (getPixRect().contains(d.getMousePosition()) && prospectBank != null)
					return true;
		}
		return false;
	}

	public void layout(NonDimension oldDim) {

		double tapeSize = getParent().getAttachArea();

		switch (getOrientation()) {

		case North:
			setNonSize(new NonDimension(oldDim.getWidth(), tapeSize));
			moveTo(new NonPosition(tapeSize, 0));
			break;
		case South:
			setNonSize(new NonDimension(oldDim.getWidth(), tapeSize));
			moveTo(new NonPosition(tapeSize, oldDim.getHeight() + tapeSize));
			break;
		case East:
			setNonSize(new NonDimension(tapeSize, oldDim.getHeight()));
			moveTo(new NonPosition(oldDim.getWidth() + tapeSize, 0 + tapeSize));
			break;
		case West:
			setNonSize(new NonDimension(tapeSize, oldDim.getHeight()));
			moveTo(new NonPosition(0, tapeSize));
			break;
		}
	}

	private boolean isActiveInsertTape() {
		Overlay o = getNonMaps().getNonLinearWorld().getViewport().getOverlay();

		for (DragProxy d : o.getDragProxies()) {
			Control r = d.getCurrentReceiver();
			if (r != null) {

				if (r instanceof PresetManager || r instanceof Tape) {
					boolean visible = super.isVisible();
					Bank b = (Bank) d.getOrigin();
					boolean bankIsNotInCluster = !b.isInCluster();

					return visible && o.isCurrentlyDraggingATypeOf(Bank.class.getName())
							&& getParent().hasSlaveInDirection(getOrientation()) && bankIsNotInCluster;
				}
			}
		}

		return false;
	}

	private Orientation getOrientation() {
		return orientation;
	}

	private void setOrientation(Orientation orientation) {
		this.orientation = orientation;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		Rect r = getPixRect().copy();
		prepareRectForDraw(r);
		r.fill(ctx, isInsertTape() ? getInsertColor() : getTapeColor());
	}

	private void prepareRectForDraw(Rect r) {
		switch (orientation) {
		case East:
			r.setWidth(r.getWidth() / 2);
			break;

		case North:
			r.setHeight(r.getHeight() / 2);
			r.setTop(r.getTop() + r.getHeight());
			break;

		case South:
			r.setHeight(r.getHeight() / 2);
			break;

		case West:
			r.setWidth(r.getWidth() / 2);
			r.setLeft(r.getLeft() + r.getWidth());
			break;

		default:
			break;

		}
	}

	private RGB getInsertColor() {
		RGB activeColor = new RGB(172, 185, 198);

		if (isCurrentlyDraggedOverMe())
			return activeColor;

		return new RGB(51, 83, 171);
	}

	private RGB getTapeColor() {
		RGB activeColor = new RGB(172, 185, 198);

		return getParent().getParent().isAttachingTape(this) ? activeColor : new RGB(51, 83, 171);
	}

	private boolean fitsTo(Tape others) {
		if (getParent().isClusteredWith(others.getParent()))
			return false;

		if (!isVisible())
			return false;

		if (!others.isVisible())
			return false;

		return isInvertedOrientation(others.orientation);
	}

	private boolean isInvertedOrientation(Orientation other) {
		return (orientation == Orientation.East && other == Orientation.West)
				|| (orientation == Orientation.West && other == Orientation.East)
				|| (orientation == Orientation.North && other == Orientation.South)
				|| (orientation == Orientation.South && other == Orientation.North);
	}

	private boolean fitsTo(Bank other) {
		if (getParent().isClusteredWith(other))
			return false;

		if (!isVisible())
			return false;

		return other.isVisible();
	}

	@Override
	public Control drag(Rect pos, DragProxy dragProxy) {
		if (isInsertTape())
			return insertTapeDrag(pos, dragProxy);

		if (isVisible() && !getParent().isDraggingControl()) {
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
		}
		return super.drag(pos, dragProxy);
	}

	@Override
	public Control drop(Position pos, DragProxy dragProxy) {
		if (isActiveInsertTape())
			return insertTapeDrop(pos, dragProxy);

		if (dragProxy.getOrigin() instanceof Bank) {
			Bank other = (Bank) dragProxy.getOrigin();
			Bank clusterMaster = other.getClusterMaster();
			DragProxy dragProxyForClusterMaster = NonMaps.get().getNonLinearWorld().getViewport().getOverlay()
					.getDragProxyFor(clusterMaster);
			Position dropPosition = dragProxyForClusterMaster != null
					? dragProxyForClusterMaster.getPixRect().getPosition()
					: pos;
			NonPosition nonPos = NonMaps.get().getNonLinearWorld().toNonPosition(dropPosition);
			nonPos.snapTo(PresetManager.getSnapGridResolution());
			NonMaps.get().getServerProxy().dockBanks(getParent(), orientation, other, nonPos);
			other.getClusterMaster().moveTo(nonPos);
			requestLayout();
			return this;
		}
		return super.drop(pos, dragProxy);
	}

	private void setShouldInsert(Bank b) {
		prospectBank = b;
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

	private boolean shouldInsert(Bank b) {
		return b == prospectBank;
	}

	private Control insertTapeDrag(Rect pos, DragProxy dragProxy) {
		if (isVisible() && !getParent().isDraggingControl()) {
			if (dragProxy.getOrigin() instanceof Bank) {
				Bank other = (Bank) dragProxy.getOrigin();

				if (getPixRect().contains(dragProxy.getMousePosition())) {
					if (getParent() != other) {
						if (fitsTo(other)) {
							setShouldInsert(other);
							return this;
						}
					}
				}
				setShouldInsert(null);
			}
		}
		return super.drag(pos, dragProxy);
	}

	private Control insertTapeDrop(Position pos, DragProxy dragProxy) {
		if (dragProxy.getOrigin() instanceof Bank) {
			Bank other = (Bank) dragProxy.getOrigin();
			if (shouldInsert(other)) {
				NonMaps.get().getServerProxy().insertBankInCluster(other, orientation, this.getParent());
				requestLayout();
			}
			return this;
		}
		return super.drop(pos, dragProxy);
	}
}
