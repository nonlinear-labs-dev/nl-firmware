package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.LoadToPartMode;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class TypeLabel extends OverlayLayout {


	private class SingleTypeLabel extends Label {

		public SingleTypeLabel(TypeLabel parent) {
			super(parent);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			Preset p = TypeLabel.this.getParent().getMapsPreset();

			if (p != null) {
				switch (p.getType()) {
					case Single:
						return "";
					case Split:
						return "\uE0F0";
					case Layer:
						return "\uE0F3";
				}
			}

			return "";
		}

		protected TextAlign getAlignment() {
			return TextAlign.LEFT;
		}
	
		@Override
		protected Rect getTextRect() {
			Rect r = super.getTextRect().copy();
			r.applyPadding(0, 0, Millimeter.toPixels(1), 0);
			return r;
		}

	}

	private class DualTypeLabel extends OverlayControl {

		public DualTypeLabel(TypeLabel parent) {
			super(parent);
		}

		private TypeLabel getTypeLabel() {
			return (TypeLabel)super.getParent();
		}

		@Override
		public void draw(Context2d ctx, int invalidationMask) {
			switch(getTypeLabel().getParent().getMapsPreset().getType()) {
				case Single:
				default:
				drawSingle(ctx);
					break;
				case Layer:
				drawLayer(ctx);
					break;
				case Split:
				drawSplit(ctx);
					break;
			}

			getPixRect().drawDebug(ctx);
		}
		
		VoiceGroup getCurrentVG() {
			return EditBufferModel.get().voiceGroup.getValue();
		}

		LoadToPartMode getLoadToPart() {
			return NonMaps.get().getNonLinearWorld().getPresetManager().getLoadToPartMode();
		}

		private void drawLayer(Context2d ctx) {
			VoiceGroup currentVG = getCurrentVG();

			LoadToPartMode mode = getLoadToPart();
			if(mode != null) {
				boolean isPresetSelected = mode.getSelectedPreset() == getTypeLabel().getParent().getMapsPreset();
				boolean iSelected = isPresetSelected && mode.getSelectedPart() == VoiceGroup.I;
				boolean iiSelected = isPresetSelected && mode.getSelectedPart() == VoiceGroup.II;

				Rect pix = getPixRect().copy();
				double height = getRelativePosition().getHeight() / 2;
				double width = getRelativePosition().getWidth();

				ctx.setFillStyle(iSelected ? "green" : "gray");
				ctx.fillRect(pix.getLeft(), pix.getTop(), width, height);

				ctx.setFillStyle(iiSelected ? "green" : "gray");
				ctx.fillRect(pix.getLeft(), pix.getTop() + height, width, height);
			}
		}

		private void drawSingle(Context2d ctx) {

		}

		private void drawSplit(Context2d ctx) {

		}
	}

	private SingleTypeLabel singleControl = null;
	private DualTypeLabel dualControl = null;

	public TypeLabel(BeltPreset parent) {
		super(parent);
		bruteForce();
    }

	private boolean isLoadToPartEnabled() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().isInLoadToPartMode();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if(dualControl != null)
			dualControl.draw(ctx, invalidationMask);
		
		if(singleControl != null)
			singleControl.draw(ctx, invalidationMask);
	}

	public void bruteForce() {
		removeAll();
		dualControl = null;
		singleControl = null;

		if(isLoadToPartEnabled()) {
			addChild(dualControl = new DualTypeLabel(this));
		} else {
			addChild(singleControl = new SingleTypeLabel(this));
		}
	}

	@Override
	public BeltPreset getParent() {
		return (BeltPreset) super.getParent();
	}
}
