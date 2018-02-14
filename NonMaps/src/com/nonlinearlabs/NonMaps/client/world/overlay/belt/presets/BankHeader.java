package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.contextStates.ClipContext;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.IBank;
import com.nonlinearlabs.NonMaps.client.world.IPreset;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.DragProxy;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.EditBufferDraggingButton;

class BankHeader extends OverlayLayout {

	private class OverlayBankContextMenu extends BankContextMenu {
		private OverlayBankContextMenu(OverlayLayout parent, Bank bank) {
			super(parent, bank);
		}

		@Override
		protected boolean hasMinimize() {
			return false;
		}

		@Override
		protected boolean hasPaste() {
			return false;
		}

		@Override
		protected boolean hasBankCreationRights() {
			return false;
		}

	}

	private OverlayControl prev;
	private OverlayControl title;
	private OverlayControl next;
	private boolean isDropTarget = false;

	BankHeader(BankControl parent) {
		super(parent);
		addChild(prev = new PreviousBank(this));
		addChild(next = new NextBank(this));
		addChild(title = new BankTitle(this));
	}

	@Override
	public BankControl getParent() {
		return (BankControl) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		if (isDropTarget)
			drawTeeth(ctx);

	}

	protected void drawTeeth(Context2d ctx) {
		try (ClipContext clipper = new ClipContext(ctx, this)) {
			drawClippedTeeth(ctx);
		}
	}

	protected void drawClippedTeeth(Context2d ctx) {
		ctx.setFillStyle(new RGBA(255, 0, 0, 0.5).toString());
		Rect r = getPixRect();

		double dim = r.getHeight() / 2;
		for (double x = r.getLeft(); x < r.getRight(); x += dim)
			drawTooth(ctx, x, r.getCenterPoint().getY() - dim / 2, dim);
	}

	private void drawTooth(Context2d ctx, double x, double y, double dim) {
		ctx.beginPath();
		ctx.moveTo(x, y);
		ctx.lineTo(x + dim, y);
		ctx.lineTo(x + dim / 2, y + dim);
		ctx.closePath();
		ctx.fill();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double border = getSpaceBetweenChildren();
		prev.doLayout(0, 0, h, h);
		next.doLayout(w - h, 0, h, h);
		double titleLeft = prev.getRelativePosition().getRight() + 0.5 + border;
		double titleWidth = next.getRelativePosition().getLeft() - titleLeft - border;
		title.doLayout(titleLeft, 0, titleWidth, h);
	}

	@Override
	public Control drag(Position pos, DragProxy dragProxy) {
		if (dragProxy.getOrigin() instanceof IPreset || dragProxy.getOrigin() instanceof EditBufferDraggingButton
				|| dragProxy.getOrigin() instanceof IBank) {
			setIsDropTarget(true);
			return this;
		}
		return super.drag(pos, dragProxy);
	}

	@Override
	public void dragLeave() {
		setIsDropTarget(false);
		super.dragLeave();
	}

	private void setIsDropTarget(boolean isDropTarget) {
		if (this.isDropTarget != isDropTarget) {
			this.isDropTarget = isDropTarget;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	@Override
	public Control drop(Position pos, DragProxy dragProxy) {
		Bank b = getParent().getBankInCharge();

		if (dragProxy.getOrigin() instanceof IPreset)
			getNonMaps().getServerProxy().dropPresetOnBank((IPreset) dragProxy.getOrigin(), b);
		else if (dragProxy.getOrigin() instanceof EditBufferDraggingButton)
			getNonMaps().getServerProxy().dropEditBufferOnBank(b);
		else if (dragProxy.getOrigin() instanceof IBank)
			getNonMaps().getServerProxy().dropBankOnBank((IBank) dragProxy.getOrigin(), b);

		setIsDropTarget(false);
		return this;
	}

	@Override
	public Control onContextMenu(Position pos) {
		if (getParent().isInStoreSelectMode())
			return null;

		Bank b = getParent().getBankInCharge();
		if (b != null) {
			Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
			return o.setContextMenu(pos, new OverlayBankContextMenu(o, b));
		}
		return super.onContextMenu(pos);
	}

	public void setFontHeightInMM(int mm) {
		((BankTitle) title).setFontHeightInMM(mm);
	}
}
