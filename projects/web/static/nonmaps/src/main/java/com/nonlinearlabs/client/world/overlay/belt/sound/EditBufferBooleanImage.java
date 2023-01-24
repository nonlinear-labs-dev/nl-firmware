package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class EditBufferBooleanImage extends SVGImage {
	public EditBufferBooleanImage(OverlayLayout parent, String imageName) {
		super(parent, imageName);
	}

	public void update(boolean b) {
		if(isVisible() != b) {
			setVisible(b);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}
}