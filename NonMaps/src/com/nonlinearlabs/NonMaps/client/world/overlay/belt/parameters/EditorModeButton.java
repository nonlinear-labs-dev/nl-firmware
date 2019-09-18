package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class EditorModeButton extends SVGImage {

	public EditorModeButton(BeltParameterLayout parent) {
		super(parent, "MC_Edit_Active.svg", "MC_Edit_Enabled.svg");
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		if (isChanged()) {
			Rect pixrect = getPixRect().copy();
			pixrect.moveBy(1, 0);
			pixrect.reduceHeightBy(-2);
			pixrect.reduceWidthBy(-2);
			pixrect.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 2, 1, null, RGB.yellow());
		}
	}

	private boolean isChanged() {
		BasicParameterModel bpm = EditBufferModel.get().getSelectedParameter();
		if (bpm instanceof ModulateableParameter) {
			ModulateableParameter modP = (ModulateableParameter) bpm;
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
