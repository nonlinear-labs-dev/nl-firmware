package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SetupPages extends OverlayLayout {

	private SetupPagesHeader header;
	private SetupPage pages[];
	private int pageIndex = 0;

	protected SetupPages(Control parent) {
		super(parent);

		addChild(header = new SetupPagesHeader(this));
		pages = header.createPages();
		addChild(pages[pageIndex]);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		double lineHeight = Millimeter.toPixels(10);
		header.doLayout(0, 0, w, lineHeight);

		for (SetupPage p : pages)
			p.doLayout(0, lineHeight, w, h - lineHeight);

		super.doLayout(x, y, w, h);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		header.drawInActiveTabs(ctx);
		drawBackground(ctx);
		super.draw(ctx, invalidationMask);
		header.drawActiveTab(ctx);
	}

	private void drawBackground(Context2d ctx) {
		Rect r = getPixRect();
		double corner = Millimeter.toPixels(2);
		double activeTabLeft = header.getActiveTabLeft();
		double activeTabRight = header.getActiveTabRight();
		double tabBottom = header.getPixRect().getBottom();

		ctx.beginPath();
		ctx.moveTo(r.getLeft(), r.getBottom() - corner);

		if (Math.abs(activeTabLeft - r.getLeft()) < 2) {
			ctx.lineTo(r.getLeft(), r.getTop() + corner);
			ctx.arcTo(r.getLeft(), r.getTop(), r.getLeft() + corner, r.getTop(), corner);
			ctx.lineTo(activeTabRight - corner, r.getTop());
			ctx.arcTo(activeTabRight, r.getTop(), activeTabRight, r.getTop() + corner, corner);
			ctx.lineTo(activeTabRight, tabBottom - corner);
			ctx.arcTo(activeTabRight, tabBottom, activeTabRight + corner, tabBottom, corner);
			ctx.lineTo(r.getRight() - corner, tabBottom);
			ctx.arcTo(r.getRight(), tabBottom, r.getRight(), tabBottom + corner, corner);
		} else if (Math.abs(activeTabRight - r.getRight()) < 2) {
			ctx.lineTo(r.getLeft(), tabBottom + corner);
			ctx.arcTo(r.getLeft(), tabBottom, r.getLeft() + corner, tabBottom, corner);
			ctx.lineTo(activeTabLeft - corner, tabBottom);
			ctx.arcTo(activeTabLeft, tabBottom, activeTabLeft, tabBottom - corner, corner);
			ctx.lineTo(activeTabLeft, r.getTop() + corner);
			ctx.arcTo(activeTabLeft, r.getTop(), activeTabLeft + corner, r.getTop(), corner);
			ctx.lineTo(activeTabRight - corner, r.getTop());
			ctx.arcTo(activeTabRight, r.getTop(), activeTabRight, r.getTop() + corner, corner);
		} else {
			ctx.lineTo(r.getLeft(), tabBottom + corner);
			ctx.arcTo(r.getLeft(), tabBottom, r.getLeft() + corner, tabBottom, corner);
			ctx.lineTo(activeTabLeft - corner, tabBottom);
			ctx.arcTo(activeTabLeft, tabBottom, activeTabLeft, tabBottom - corner, corner);
			ctx.lineTo(activeTabLeft, r.getTop() + corner);
			ctx.arcTo(activeTabLeft, r.getTop(), activeTabLeft + corner, r.getTop(), corner);
			ctx.lineTo(activeTabRight - corner, r.getTop());
			ctx.arcTo(activeTabRight, r.getTop(), activeTabRight, r.getTop() + corner, corner);
			ctx.lineTo(activeTabRight, tabBottom - corner);
			ctx.arcTo(activeTabRight, tabBottom, activeTabRight + corner, tabBottom, corner);
			ctx.lineTo(r.getRight() - corner, tabBottom);
			ctx.arcTo(r.getRight(), tabBottom, r.getRight(), tabBottom + corner, corner);
		}

		ctx.lineTo(r.getRight(), r.getBottom() - corner);
		ctx.arcTo(r.getRight(), r.getBottom(), r.getRight() - corner, r.getBottom(), corner);
		ctx.lineTo(r.getLeft() + corner, r.getBottom());
		ctx.arcTo(r.getLeft(), r.getBottom(), r.getLeft(), r.getBottom() - corner, corner);

		ctx.closePath();

		ctx.setFillStyle(new RGB(32, 32, 32).toString());
		ctx.fill();

		ctx.setLineWidth(Millimeter.toPixels(0.25));
		ctx.setStrokeStyle(new Gray(99).toString());
		ctx.stroke();
	}

	public void onTabSwitched(int pageIndex) {
		removeChild(pages[this.pageIndex]);
		this.pageIndex = pageIndex;
		addChild(pages[this.pageIndex]);
		requestLayout();
	}

	public PresetDragDropSetting getPresetDragDropSetting() {
		GUISettingsPage p = (GUISettingsPage) pages[1];
		return p.getPresetDragDropSetting();
	}

	public ContextMenusSetting getContextMenuSettings() {
		GUISettingsPage p = (GUISettingsPage) pages[1];
		return p.getContextMenuSettings();
	}

	public EditParameterSetting getEditParameterDragSetting() {
		GUISettingsPage p = (GUISettingsPage) pages[1];
		return p.getEditParameterDragSetting();
	}
}
