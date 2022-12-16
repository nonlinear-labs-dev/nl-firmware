package com.nonlinearlabs.client.world.overlay.undo;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.FloatingWindow;
import com.nonlinearlabs.client.world.overlay.FloatingWindowHeader;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.html.overlayControlDialog.OverlayControlDialog;

public class UndoTreeWindow extends FloatingWindow {
	private OverlayControlDialog dialog = new OverlayControlDialog(this, "undo-tree-window");
	private UndoTree content;
	private Dimension fixDimension;
	private FloatingWindowHeader header;
	private boolean scheduledScrollingToCurrentUndoStep = true;
	private static boolean firstTimeOpen = true;

	public UndoTreeWindow(Overlay parent) {
		super(parent);
		fixDimension = new Dimension(NonMaps.mmToPixels(70), NonMaps.mmToPixels(70));

		addChild(header = new FloatingWindowHeader(this));
		addChild(content = new UndoTree(this));

		dialog.show();
	}

	@Override
	public void doLayout(double x, double y) {
		double lineHeight = Millimeter.toPixels(10);

		if (isHidden()) {
			super.doLayout(x, y, 0, 0);

		} else {
			double margin = Millimeter.toPixels(2);
			header.doLayout(0, 0, fixDimension.getWidth(), lineHeight);
			content.doLayout(margin, lineHeight, fixDimension.getWidth() - 2 * margin,
					fixDimension.getHeight() - lineHeight - margin);
			super.doLayout(x, y, fixDimension.getWidth(), fixDimension.getHeight());
		}
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
	public void onRemoved() {
		super.onRemoved();
		dialog.hide();
	}

	@Override
	public void calcPixRect(Position parentsReference, double currentZoom) {
		super.calcPixRect(parentsReference, currentZoom);

		if (scheduledScrollingToCurrentUndoStep) {
			scheduledScrollingToCurrentUndoStep = false;
			content.sanitizeScrollPosition();
			content.scrollToCurrentUndoStep(false);
		}
	}

	@Override
	public void draw(Context2d c, int invalidationMask) {
		Context2d ctx = dialog.getContext();
		Rect r = getPixRect();

		ctx.save();
		ctx.translate(-r.getLeft(), -r.getTop());

		super.draw(ctx, invalidationMask);
		ctx.restore();
	}

	@Override
	public String getTitle() {
		return "Undo History";
	}

	public void update(Node undoNode) {
		content.update(undoNode);
	}

	@Override
	public void toggle() {
		if (isHidden() && firstTimeOpen) {
			double width = NonMaps.get().getCanvas().getCoordinateSpaceWidth() / 4;
			double height = NonMaps.get().getCanvas().getCoordinateSpaceHeight() / 2;
			fixDimension = new Dimension(width, height);
			firstTimeOpen = false;
		}

		super.toggle();

		if (isShown())
			scheduledScrollingToCurrentUndoStep = true;
	}

	public UndoTree getTree() {
		return content;
	}

	public void onDragHandleMoved(double x, double y) {
		double min = Millimeter.toPixels(30);
		fixDimension.setWidth(Math.max(min, fixDimension.getWidth() + x));
		fixDimension.setHeight(Math.max(min, fixDimension.getHeight() + y));
		requestLayout();
	}

}
