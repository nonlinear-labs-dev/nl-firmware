package com.nonlinearlabs.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public abstract class RecallButton extends OverlayControl {
	boolean active;

	public RecallButton(OverlayLayout parent) {
		super(parent);
		active = false;
	}

	public boolean isActive() {
		return active;
	}

	public void setActive(boolean b) {
		active = b;
	}


}
