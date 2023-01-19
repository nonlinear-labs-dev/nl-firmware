package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

class SerialArrow extends SVGImage {
	int selectedPhase = 0;

	SerialArrow(OverlayLayout parent) {
		super(parent, "CT-to-CB.svg", "CB-to-CT.svg");
		setVisible(false);

		EditBufferPresenterProvider.get().onChange(ebp -> {
			switch(ebp.serialFX) {
				case None:
					setVisible(false);
				break;
				case FX_I_IN_II:
					selectedPhase = 0;
					invalidate(INVALIDATION_FLAG_UI_CHANGED);
					setVisible(true);
				break;
				case FX_II_IN_I:
					selectedPhase = 1;
					invalidate(INVALIDATION_FLAG_UI_CHANGED);
					setVisible(true);
					break;
			}
			return true;
		});
	}

	@Override
	public int getSelectedPhase() {
		return selectedPhase;
	}
}