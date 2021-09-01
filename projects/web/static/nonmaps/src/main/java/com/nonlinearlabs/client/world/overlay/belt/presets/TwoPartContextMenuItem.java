package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.PresetContextMenuItem;

public class TwoPartContextMenuItem extends PresetContextMenuItem {

	protected String endString;

	public TwoPartContextMenuItem(ContextMenu presetContextMenu, String string, String end) {
		super(presetContextMenu, string);
		endString = end;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		ctx.fillText(endString, getPixRect().getRight() - ctx.measureText(endString).getWidth(),
				getPixRect().getTop() + getPixRect().getHeight() / 2);
	}

}
