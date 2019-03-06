package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
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

	private boolean isChanged() {
		BasicParameterModel bpm = EditBufferModel.get().getSelectedParameter();
		
		if(bpm instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter)bpm;
			if(mode == Mode.mcAmount) {
				return modP.ogModAmount.getValue() != modP.modAmount.getValue();
			} else if(mode == Mode.mcValue) {
				return modP.isMCPosChanged();
			}
		}
		return false;
	}
	
	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		if(isChanged()) {
			Rect pixRect = getPixRect().copy();
			pixRect.reduceHeightBy(pixRect.getHeight() / 2.2);
			pixRect.reduceWidthBy(pixRect.getWidth() / 3);
			pixRect.moveBy(1, 0.5);
			pixRect.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 19, 1, null, RGB.yellow());
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