package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

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

		double buttonWidth = SVGImage.calcSVGDimensionToPixels(36);
		double buttonHeight = SVGImage.calcSVGDimensionToPixels(34);
		double margin = getButtonDimension() / 8;
		double top = (h - 2 * buttonHeight + margin) / 3;

		prev.doLayout(0, top, buttonWidth, buttonHeight);
		next.doLayout(0, prev.getRelativePosition().getBottom() + margin, buttonWidth, buttonHeight);
	}
}
