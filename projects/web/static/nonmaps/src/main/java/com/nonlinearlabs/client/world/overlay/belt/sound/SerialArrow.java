package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SerialFX;
import com.nonlinearlabs.client.tools.Pair;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

class SerialArrow extends MappedSvgImage<SerialFX> {

	SerialArrow(OverlayLayout parent) {
		super(parent, new Pair<SerialFX, String>(SerialFX.FX_I_IN_II, "CT-to-CB.svg"), 
					  new Pair<SerialFX, String>(SerialFX.FX_II_IN_I, "CB-to-CT.svg"), 
					  new Pair<SerialFX, String>(SerialFX.None, null));

		setVisible(false);

		EditBufferPresenterProvider.get().onChange(ebp -> {
			update(ebp.serialFX);
			return true;
		});
	}
}