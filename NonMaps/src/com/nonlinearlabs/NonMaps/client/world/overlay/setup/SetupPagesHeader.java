package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SetupPagesHeader extends OverlayLayout {

	private Tab active;
	private Tab right;

	protected SetupPagesHeader(SetupPages parent) {
		super(parent);

		addChild(active = new Tab(this, 0, "Device Settings") {

			@Override
			public SetupPage createPage(SetupPages setupPages) {
				return new DeviceSettingsPage(setupPages);
			}
		});

		addChild(new Tab(this, 1, "GUI Settings") {

			@Override
			public SetupPage createPage(SetupPages setupPages) {
				return new GUISettingsPage(setupPages);
			}
		});

		addChild(new Tab(this, 2, "System Info") {

			@Override
			public SetupPage createPage(SetupPages setupPages) {
				return new SystemInfoPage(setupPages);
			}
		});

		addChild(right = new Tab(this, 3, "About") {

			@Override
			public SetupPage createPage(SetupPages setupPages) {
				return new AboutSettingsPage(setupPages);
			}
		});
	}

	@Override
	public SetupPages getParent() {
		return (SetupPages) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double margin = Millimeter.toPixels(0.5);
		double xPos = 0;
		double width = (w - 3 * margin) / 4;

		for (OverlayControl c : getChildren()) {
			c.doLayout(xPos, 0, width, h);
			xPos += width + margin;
		}

		right.getRelativePosition().setRight(getRelativePosition().getRight());
	}

	public void onTabClicked(Tab page, int pageIndex) {
		if (active != page) {
			active = page;
			getParent().onTabSwitched(pageIndex);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	public void drawActiveTab(Context2d ctx) {
		active.draw(ctx, INVALIDATION_FLAG_UI_CHANGED);
	}

	public boolean isActiveTab(Tab tab) {
		return active == tab;
	}

	public double getActiveTabLeft() {
		return active.getPixRect().getLeft();
	}

	public double getActiveTabRight() {
		return active.getPixRect().getRight();
	}

	public Tab getActiveTab() {
		return active;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
	}

	public void drawInActiveTabs(Context2d ctx) {
		for (OverlayControl c : getChildren()) {
			if (c != active)
				c.draw(ctx, INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	public SetupPage[] createPages() {
		SetupPage[] pages = new SetupPage[4];

		for (int i = 0; i < 4; i++) {
			Tab t = (Tab) getChildren().get(i);
			pages[i] = t.createPage(getParent());
		}
		return pages;
	}

}
