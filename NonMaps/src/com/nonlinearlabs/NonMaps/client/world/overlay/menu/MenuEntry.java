package com.nonlinearlabs.NonMaps.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.Tracer;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class MenuEntry extends Label {

	private class ShortCutShow extends Label {

		String text;
		MenuEntry daddyentry;

		public ShortCutShow(OverlayLayout parent, MenuEntry daddyentry, String text) {
			super(parent);
			this.text = text;
			this.daddyentry = daddyentry;
			this.setFontColor(new Gray(150));
		}

		@Override
		public String getDrawText(Context2d ctx) {
			Rect r = daddyentry.getPixRect().copy();
			r.setRight(r.getRight() - 10);
			this.setPixRect(r);
			return text;
		}

		@Override
		protected TextAlign getAlignment() {
			return TextAlign.RIGHT;
		}

	};

	private ShortCutShow shortcutLabel;

	public MenuEntry(GlobalMenu menu, String shortcut) {
		super(menu);
		shortcutLabel = new ShortCutShow(this.getParent(), this, shortcut);
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
		shortcutLabel.draw(ctx, invalidationMask);
	}

}
