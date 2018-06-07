package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;

public class EditSmoothingTimeSetting extends Setting {

	protected EditSmoothingTimeSetting(DeviceSettingsPage parent) {
		super(parent, "Edit Smoothing Time");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new NumericValueControl(this, "0 ms") {

			@Override
			public Control click(Position eventPoint) {
				Rect leftRect = getPixRect().copy();
				leftRect.setRight(getPixRect().getLeft() + getPixRect().getWidth() / 2);
				Rect rightRect = getPixRect().copy();
				rightRect.setLeft(getPixRect().getRight() - getPixRect().getWidth() / 2);

				if (leftRect.contains(eventPoint)) {
					value.dec(Initiator.EXPLICIT_USER_ACTION, false);
					return this;
				} else if (rightRect.contains(eventPoint)) {
					value.inc(Initiator.EXPLICIT_USER_ACTION, false);
					return this;
				}
				return super.click(eventPoint);
			}

			@Override
			protected void sendToServer(double v) {
				NonMaps.theMaps.getServerProxy().setSetting("EditSmoothingTime", Double.toString(v));
			}
		};
	}
}
