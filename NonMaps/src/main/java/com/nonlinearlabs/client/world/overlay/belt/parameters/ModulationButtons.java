package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class ModulationButtons extends OverlayLayout {

	protected Label originalSource;

	protected class OGLabel extends Label {

		public OGLabel(OverlayLayout parent) {
			super(parent);
			setFontColor(RGB.changedText());
			setFontHeightInMM(5);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			BasicParameterModel bpm = EditBufferModel.getSelectedParameter();
			if (bpm instanceof ModulateableParameterModel) {
				ModulateableParameterModel modP = (ModulateableParameterModel) bpm;
				if (modP.isModSourceChanged())
					return "(" + modP.ogModSource.getValue().toString() + ")";
			}
			return "";
		}
	}

	public ModulationButtons(BeltParameterLayout parent) {
		super(parent);

		MacroControls buttons[] = { MacroControls.A, MacroControls.B, MacroControls.C, MacroControls.D };

		for (MacroControls b : buttons) {
			addChild(new ModulationSourceButton(this, b));
		}

		addChild(originalSource = new OGLabel(this));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);
		if (isChanged()) {
			Rect pix = getPixRect().copy();
			pix = pix.getReducedBy(pix.getWidth() / -5.5);
			pix.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 5, 1.5, null, RGB.changedBeltBorder());
		}
	}

	private boolean isChanged() {
		BasicParameterModel bpm = EditBufferModel.getSelectedParameter();
		if (bpm instanceof ModulateableParameterModel) {
			return ((ModulateableParameterModel) bpm).isModSourceChanged();
		}
		return false;
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public void doLayout(double posx, double posy, double w, double h) {
		super.doLayout(posx, posy, w, h);

		double buttonDim = Millimeter.toPixels(10);
		double yspace = (h - 2 * buttonDim);
		double xspace = (w - 2 * buttonDim);

		double space = Math.min(xspace, yspace);
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
				int index = y * 2 + x;
				getChildren().get(index).doLayout(x * (space + buttonDim), y * (space + buttonDim), buttonDim,
						buttonDim);
			}
		}

		originalSource.doLayout(0, buttonDim - buttonDim * 0.25, w, buttonDim);
	}
}
