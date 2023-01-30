package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

class FX_II_Indicator extends SVGImage {

	FX_II_Indicator(OverlayLayout parent) {
		super(parent, "FX-II-active.svg", "FX-II-inactive.svg");
	
		EditBufferPresenterProvider.get().onChange(e -> {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return true;
		});
	}

	@Override
	public int getSelectedPhase() {
		VoiceGroup currentVG = EditBufferPresenterProvider.getPresenter().voiceGroupEnum;
		return currentVG == VoiceGroup.I ? 1 : 0;
	}

	@Override
	public Control click(Position p) {
		EditBufferUseCases.get().selectVoiceGroup(VoiceGroup.II);
		return this;
	}
}