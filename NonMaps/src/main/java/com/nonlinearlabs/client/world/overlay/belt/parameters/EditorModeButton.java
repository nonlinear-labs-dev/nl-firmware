package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class EditorModeButton extends SVGImage {

	public EditorModeButton(BeltParameterLayout parent) {
		super(parent, "MC_Edit_Active.svg", "MC_Edit_Enabled.svg");
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		if (isChanged()) {
			Rect pix = getPixRect().copy();
			pix = pix.getReducedBy(pix.getWidth() * -0.5);
			pix.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 5, 1.5, null, RGB.changedBeltBorder());
		}
	}

	private boolean isChanged() {
		BasicParameterModel bpm = EditBufferModel.getSelectedParameter();
		if (bpm instanceof ModulateableParameterModel) {
			ModulateableParameterModel modP = (ModulateableParameterModel) bpm;
			return modP.isModAmountChanged() || modP.isModSourceChanged() || modP.isMCPosChanged();
		}
		return false;
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		if (isActive())
			getParent().setMode(BeltParameterLayout.Mode.modulateableParameter);
		else
			getParent().setMode(BeltParameterLayout.Mode.paramValue);

		return this;
	}

	@Override
	public boolean isVisible() {
		if (getParent().isOneOf(BeltParameterLayout.Mode.unmodulateableParameter))
			return false;

		return super.isVisible();
	}

	@Override
	public int getSelectedPhase() {
		boolean active = isActive();
		return active ? 0 : 1;
	}

	private boolean isActive() {
		return getParent().isOneOf(BeltParameterLayout.Mode.mcValue, BeltParameterLayout.Mode.mcAmount,
				BeltParameterLayout.Mode.mcSource, BeltParameterLayout.Mode.paramValue,
				BeltParameterLayout.Mode.mcLower, BeltParameterLayout.Mode.mcUpper);
	}

	@Override
	public Control doubleClick() {
		return this;
	}
}
