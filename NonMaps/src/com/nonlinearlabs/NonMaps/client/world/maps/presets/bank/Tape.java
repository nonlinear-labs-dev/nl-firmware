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
		return isActiveEmptyTape() || isActiveInsertTape();
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
		if(!isInsertTape())
			return false;

		Overlay o = getNonMaps().getNonLinearWorld().getViewport().getOverlay();
		for (DragProxy d : o.getDragProxies()) {
			Control r = d.getCurrentReceiver();
			if (r == this)
				if(getPixRect().contains(d.getMousePosition()) && prospectBank != null)
					return true;
		}
		return false;
	}

	public void layout(NonDimension oldDim) {

		double tapeSize = getParent().getAttachArea();

		switch(getOrientation()) {

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
					return visible && 
							o.isCurrentlyDraggingATypeOf(Bank.class.getName()) &&
							getParent().hasSlaveInDirection(getOrientation());
				}
			}
		}

		return false;
	}

	public Orientation getOrientation() {
		return orientation;
	}

	public void setOrientation(Orientation orientation) {
		this.orientation = orientation;
	}

	public Bank getBankInOrientation(Orientation o) {
		switch(o) {
			case North:
				return getParent().getMasterTop();
			case South:
				return getParent().getBottomSlave();
			case East:
				return getParent().getRightSlave();
			case West:
				return getParent().getMasterLeft();
		}
		return null;
	}

	private double getMidPoint(double v, double v2) {
		return (v + v2) / 2.0;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		Rect r = getPixRect().copy();
		if(isInsertTape()) {
			ctx.beginPath();
			Position begin = new Position();
			Position end = new Position();
			Rect bankRect = getPixRect().copy();
			Bank b = getBankInOrientation(getOrientation());
			Rect bRect = b.getPixRect().copy();
			switch(getOrientation()) {
				case North:
					begin.setX(getMidPoint(bankRect.getLeft(), bRect.getLeft()));
					begin.setY(getMidPoint(bankRect.getTop(), bRect.getBottom()));
					end.setX(getMidPoint(bankRect.getRight(), bRect.getRight()));
					end.setY(getMidPoint(bankRect.getTop(), bRect.getBottom()));
					break;
				case South:
					begin.setX(getMidPoint(bankRect.getLeft(), bRect.getLeft()));
					begin.setY(getMidPoint(bankRect.getBottom(), bRect.getTop()));
					end.setX(getMidPoint(bankRect.getRight(), bankRect.getRight()));
					end.setY(getMidPoint(bankRect.getBottom(), bRect.getTop()));
					break;
				case East:
					begin.setX(getMidPoint(bankRect.getRight(), bRect.getLeft()));
					begin.setY(getMidPoint(bankRect.getTop(), bRect.getTop()));
					end.setX(getMidPoint(bankRect.getRight(), bRect.getLeft()));
					end.setY(Math.max(bankRect.getBottom(), bRect.getBottom()));
					break;
				case West:
					begin.setX(getMidPoint(bankRect.getLeft(), bRect.getRight()));
					begin.setY(getMidPoint(bankRect.getTop(), bRect.getTop()));
					end.setX(getMidPoint(bankRect.getLeft(), bRect.getRight()));
					end.setY(Math.max(bankRect.getBottom(), bRect.getBottom()));
					break;
			}
			if(isCurrentlyDraggedOverMe()) {
				if(getOrientation() == Orientation.North || getOrientation() == Orientation.South)
					ctx.setLineWidth(getPixRect().getHeight());
				else
					ctx.setLineWidth(getPixRect().getWidth());
			} else {
				if(getOrientation() == Orientation.North || getOrientation() == Orientation.South)
					ctx.setLineWidth(getPixRect().getHeight() / 5);
				else
					ctx.setLineWidth(getPixRect().getWidth() / 5);
			}
			ctx.moveTo(begin.getX(), begin.getY());

			ctx.setStrokeStyle(getInsertColor().toString());

			ctx.lineTo(end.getX(), end.getY());
			ctx.stroke();
			return;
		} else if(isInsertTape()) {
			if(getOrientation() == Orientation.East) {
				r.setLeft(r.getRight());
				r.setWidth(r.getWidth() / 2);
			} else if(getOrientation() == Orientation.South) {
				r.setTop(r.getBottom());
				r.setHeight(r.getHeight() / 2);
			} else {
				return;
			}
		} else {
			getPixRect().fill(ctx, getTapeColor());
		}
		r.fill(ctx, getTapeColor());
	}

	private RGB getInsertColor() {
		return new RGB(250, 250, 250);
	}

	private RGB getTapeColor() {
		RGB activeColor = new RGB(172, 185, 198);

		return getParent().getParent().isAttachingTape(this) ? activeColor : new RGB(51, 83, 171);
	}

	public boolean fitsTo(Tape others) {
		if (getParent().isClusteredWith(others.getParent()))
			return false;

		if (!isVisible())
			return false;

		if (!others.isVisible())
			return false;

		return (orientation == Orientation.East && others.orientation == Orientation.West)
				|| (orientation == Orientation.West && others.orientation == Orientation.East)
				|| (orientation == Orientation.North && others.orientation == Orientation.South)
				|| (orientation == Orientation.South && others.orientation == Orientation.North);
	}

	public boolean fitsTo(Bank other) {
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

	private Orientation invert(Orientation o) {
		switch (o) {
			case North:
				return Orientation.South;
			case South:
				return Orientation.North;
			case East:
				return Orientation.West;
			case West:
				return Orientation.East;
		}
		return null;
	}

	public Control insertTapeDrag(Rect pos, DragProxy dragProxy) {
		if (isVisible() && !getParent().isDraggingControl()) {
			if (dragProxy.getOrigin() instanceof Bank) {
				Bank other = (Bank) dragProxy.getOrigin();

				if(getPixRect().contains(dragProxy.getMousePosition())) {
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

	public Control insertTapeDrop(Position pos, DragProxy dragProxy) {
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
