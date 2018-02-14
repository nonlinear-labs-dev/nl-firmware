package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class LoadButtonArea extends OverlayLayout {

	private PrevNextPresetButtons prevNext;
	private LoadPreset load;

	protected LoadButtonArea(BeltPresetLayout parent) {
		super(parent);

		addChild(prevNext = new PrevNextPresetButtons(this));
		addChild(load = new LoadPreset(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		double margin = getButtonDimension() / 2;

		double buttonWidth = SVGImage.calcSVGDimensionToPixels(36);
		double buttonHeight = SVGImage.calcSVGDimensionToPixels(34);
		prevNext.doLayout(0, 0, buttonWidth, h);
		load.doLayout(prevNext.getRelativePosition().getRight() + margin, (h - buttonHeight) / 2, buttonWidth, buttonHeight);

		w = load.getRelativePosition().getRight() - prevNext.getRelativePosition().getLeft();
		super.doLayout(x, y, w, h);
	}

	public PrevNextPresetButtons getPrevNext() {
		return prevNext;
	}

}
