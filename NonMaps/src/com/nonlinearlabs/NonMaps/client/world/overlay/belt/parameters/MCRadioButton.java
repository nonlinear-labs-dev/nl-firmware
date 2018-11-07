package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

class MCRadioButton extends SVGImage {
	Mode mode;

	MCRadioButton(BeltParameterLayout parent, Mode m, String... imageNames) {
		super(parent, imageNames);
		this.mode = m;
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public boolean isVisible() {
		if (super.isVisible()) {
			return getPixRect().getWidth() > Millimeter.toPixels(10);
		}
		return false;
	}

	@Override
	public Control click(Position eventPoint) {
		getParent().toggleMcEditMode(this.mode);
		return this;
	}
	
	@Override
	public Control doubleClick() {
		return this;
	}

	@Override
	public int getSelectedPhase() {
		return getParent().getMode() == this.mode ? 0 : 1;
	}
}