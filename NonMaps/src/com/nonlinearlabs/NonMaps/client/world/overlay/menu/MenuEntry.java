package com.nonlinearlabs.NonMaps.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;

public abstract class MenuEntry extends Label {

	public MenuEntry(GlobalMenu menu) {
		super(menu);
	}

	@Override
	public GlobalMenu getParent() {
		return (GlobalMenu) super.getParent();
	}

	protected TextAlign getAlignment() {
		return TextAlign.LEFT;
	}

	@Override
	protected Rect getTextRect() {
		Rect r = super.getTextRect().copy();
		r.applyPadding(Millimeter.toPixels(2.5), 0, 0, 0);
		return r;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().fill(ctx, Gray.overlayBackground());
		super.draw(ctx, invalidationMask);
	}

}
