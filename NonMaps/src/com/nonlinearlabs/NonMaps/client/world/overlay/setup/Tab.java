package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;

public abstract class Tab extends CenteredSetupLabel {

	private int page = 0;

	public Tab(SetupPagesHeader parent, int page, String text) {
		super(parent, text);
		this.page = page;
	}

	@Override
	public SetupPagesHeader getParent() {
		return (SetupPagesHeader) super.getParent();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		getParent().onTabClicked(this, page);
		return this;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (!getParent().isActiveTab(this))
			drawInactiveBackground(ctx);

		super.draw(ctx, invalidationMask);
	}

	private void drawInactiveBackground(Context2d ctx) {
		Rect r = getPixRect().copy();
		r.setHeight(r.getHeight() + Millimeter.toPixels(5));
		r.drawRoundedRect(ctx, Rect.ROUNDING_LEFT_TOP | Rect.ROUNDING_RIGHT_TOP, Millimeter.toPixels(2), Millimeter.toPixels(0.25),
				new Gray(29), new Gray(12));
	}

	public abstract SetupPage createPage(SetupPages setupPages);

}
