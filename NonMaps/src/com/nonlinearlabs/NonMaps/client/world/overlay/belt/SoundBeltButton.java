package com.nonlinearlabs.NonMaps.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SoundBeltButton extends OverlayLayout implements IActivatable {

	private class SoundIcon extends BeltButton {

		private SoundIcon(OverlayLayout parent, Belt belt) {
			super(parent, belt, "Sound_Tab_Enabled_A.svg", "Sound_Tab_Disabled_A.svg");
		}

		@Override
		public Control click(Position eventPoint) {
			if (!isActive()) {
				belt.setSoundView(true);
			} else {
				belt.toggle();
			}
			return this;
		}

		@Override
		public boolean isActive() {
			return belt.isSoundView();
		}
	}

	private SoundIcon icon;

	public SoundBeltButton(OverlayLayout parent, Belt belt) {
		super(parent);
		icon = addChild(new SoundIcon(this, belt));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		icon.doLayout(0, 0, w, h);
	}

	@Override
	public Control click(Position eventPoint) {
		return icon.click(eventPoint);
	}

	@Override
	public boolean isActive() {
		return icon.isActive();
	}

}
