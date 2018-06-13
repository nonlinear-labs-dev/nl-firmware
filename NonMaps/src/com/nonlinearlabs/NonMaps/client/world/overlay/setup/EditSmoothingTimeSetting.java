package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class EditSmoothingTimeSetting extends Setting {

	protected EditSmoothingTimeSetting(DeviceSettingsPage parent) {
		super(parent, "Edit Smoothing Time");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().systemSettings.editSmoothingTime.onChange(new Function<Double, Boolean>() {

			@Override
			public Boolean apply(Double t) {
				((NumericValueControl) getSettingsControl()).update(Setup.get().systemSettings.editSmoothingTime);
				return true;
			}
		});
	}

	@Override
	protected OverlayControl createSettingsControl() {
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
				SystemSettings.get().setEditSmoothingTime(v);
			}
		};
	}
}
