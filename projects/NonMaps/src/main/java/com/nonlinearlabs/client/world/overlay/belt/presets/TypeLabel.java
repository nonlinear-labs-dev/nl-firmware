package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
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

		public DualTypeLabel(Control parent) {
			super(parent);
		}

		@Override
		public void draw(Context2d ctx, int invalidationMask) {

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

	private void bruteForce() {
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
