package com.nonlinearlabs.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public abstract class MenuEntry extends OverlayLayout {

	private class ShortcutLabel extends Label {

		String text;

		public ShortcutLabel(MenuEntry parent, String text) {
			super(parent);
			this.text = text;
			this.setFontColor(new Gray(150));
		}

		@Override
		public MenuEntry getParent() {
			return (MenuEntry) super.getParent();
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return text;
		}

		@Override
		protected TextAlign getAlignment() {
			return TextAlign.RIGHT;
		}

		@Override
		protected Rect getTextRect() {
			return super.getTextRect().getMovedBy(new Dimension(-Millimeter.toPixels(3), 0));
		}
	};

	private class EntryLabel extends Label {

		public EntryLabel(OverlayLayout parent) {
			super(parent);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return MenuEntry.this.getDrawText(ctx);
		}

		@Override
		protected TextAlign getAlignment() {
			return TextAlign.LEFT;
		}
	}

	private class CheckedLabel extends Label {

		public CheckedLabel(OverlayLayout parent) {
			super(parent);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return MenuEntry.this.getCheckText();
		}

		@Override
		protected TextAlign getAlignment() {
			return TextAlign.CENTER;
		}
	}

	private CheckedLabel checkmark;
	private EntryLabel label;
	private ShortcutLabel shortcutLabel;

	public MenuEntry(GlobalMenu menu, String shortcut) {
		super(menu);
		addChild(checkmark = new CheckedLabel(this));
		addChild(label = new EntryLabel(this));
		addChild(shortcutLabel = new ShortcutLabel(this, shortcut));
	}

	public String getCheckText() {
		return "\uE0A3 ";
	}

	@Override
	public GlobalMenu getParent() {
		return (GlobalMenu) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double checkMarkWidth = Millimeter.toPixels(9);
		checkmark.doLayout(checkMarkWidth / 2.8, 1, checkMarkWidth, h);

		double shortcutWidth = Millimeter.toPixels(20);
		shortcutLabel.doLayout(w - shortcutWidth, 0, shortcutWidth, h);

		label.doLayout(checkmark.getRelativePosition().getRight(), 0,
				shortcutLabel.getRelativePosition().getLeft() - checkmark.getRelativePosition().getRight(), h);
	}

	public abstract String getDrawText(Context2d ctx);

}
