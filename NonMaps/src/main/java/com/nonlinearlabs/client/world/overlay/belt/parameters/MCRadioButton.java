package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

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

	private boolean isChanged() {
		BasicParameterModel bpm = EditBufferModel.get().getSelectedParameter();
		if (bpm instanceof ModulateableParameter) {
			return ((ModulateableParameter) bpm).isChanged(this.mode);
		}
		return false;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		if(isChanged()) {
			Rect pix = getPixRect().copy();
			pix = pix.getReducedBy(pix.getWidth() * 0.2);
			pix.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 5, 1.5, null, RGB.changedBeltBorder());
		}
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
