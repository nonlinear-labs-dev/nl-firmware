package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.nonlinearlabs.NonMaps.client.world.Control;

abstract class UndoRedoButton extends SVGImage {

	protected boolean enabled = false;

	UndoRedoButton(Control parent, String disabled, String enabled, String touched) {
		super(parent, disabled, enabled, touched);
	}

	protected void setEnabled(boolean enabled) {
		if (this.enabled != enabled) {
			this.enabled = enabled;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	public int getSelectedPhase() {
		if (!enabled)
			return 0;

		if (isCaptureControl())
			return 2;

		return 1;
	}
}