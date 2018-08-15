package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Dimension;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
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

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		getPixRect().fill(ctx, getTapeColor());
	}

	private RGB getTapeColor() {
		RGB activeColor = new RGB(173, 181, 217);

		if (prospectBank != null)
			activeColor = new RGB(250, 250, 250);

		return getParent().getParent().isAttachingTape(this) ? activeColor : new RGB(98, 113, 183);
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
		if (isActiveInsertTape())
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

	public Control insertTapeDrag(Rect pos, DragProxy dragProxy) {
		if (isVisible() && !getParent().isDraggingControl()) {
			if (dragProxy.getOrigin() instanceof Bank) {
				Bank other = (Bank) dragProxy.getOrigin();
				Position mouse = dragProxy.getMousePosition();

				if (getPixRect().contains(mouse)) {
					if (getParent() != other) {
						if (fitsTo(other)) {
							// Display attaching!
							setShouldInsert(other);
							getParent().getParent().setAttachingTapes(this, null);
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
