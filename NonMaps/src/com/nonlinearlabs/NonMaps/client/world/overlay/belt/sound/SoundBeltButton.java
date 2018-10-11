package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

public abstract class SoundBeltButton extends Label {

	public SoundBeltButton(OverlayLayout parent) {
		super(parent);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		RGB strokeColor = new Gray(86);

		if (PointerState.get().isCurrentReceiver(this)) {
			strokeColor = RGB.blue();
		}

		getPixRect().drawRoundedArea(ctx, Millimeter.toPixels(2.5), 1, new Gray(68), strokeColor);

		super.draw(ctx, invalidationMask);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		return this;
	}

	@Override
	public void onMouseLost() {
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		super.onMouseLost();
	}
}