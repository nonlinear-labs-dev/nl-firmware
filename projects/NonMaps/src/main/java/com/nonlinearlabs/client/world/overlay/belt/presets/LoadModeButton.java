package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.presenters.PresetManagerPresenterProvider;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class LoadModeButton extends SVGImage {

	LoadModeButton(OverlayLayout parent) {
		super(parent, "Link_Enabled.svg", "Link_Active.svg", "Link_Disabled.svg");
	}

	public boolean isInStoreSelectMode() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		double imgHeight = getSelectedImage().getImgHeight();
		y = (h - imgHeight) / 2;
		y += Millimeter.toPixels(3);
		h = imgHeight;
		super.doLayout(x, y, w, h);
	}

	@Override
	public int getSelectedPhase() {
		if (isInStoreSelectMode())
			return drawStates.disabled.ordinal();
		else if (isDirectLoadActive())
			return drawStates.active.ordinal();

		return drawStates.normal.ordinal();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		if (isInStoreSelectMode())
			return this;

		toggle();
		return this;
	}

	public boolean isDirectLoadActive() {
		return PresetManagerPresenterProvider.get().getPresenter().directLoadActive;
	}

	public void toggle() {
		EditBufferUseCases.get().toggleDirectLoad();
	}
}
