package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

class PrevNextPresetButtons extends OverlayLayout {

	private PreviousPreset prev;
	private NextPreset next;

	PrevNextPresetButtons(OverlayLayout parent) {
		super(parent);
		addChild(prev = new PreviousPreset(this));
		addChild(next = new NextPreset(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonWidth = prev.getSelectedImage().getImgWidth();
		double buttonHeight = prev.getSelectedImage().getImgHeight();
		double margin = Millimeter.toPixels(2.5);

		prev.doLayout(0, 0, buttonWidth, buttonHeight);
		next.doLayout(0, prev.getRelativePosition().getBottom() + margin, buttonWidth, buttonHeight);
	}
}
