package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.Renameable;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RenameDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class DeviceNameSettings extends OverlayLayout implements Renameable {

	private DeviceNameLabel label;
	private DeviceNameEditButton button;

	protected DeviceNameSettings(DeviceNameSetting parent) {
		super(parent);
		addChild(label = new DeviceNameLabel(this));
		addChild(button = new DeviceNameEditButton(this) {
			@Override
			public Control click(Position eventPoint) {
				RenameDialog.open(DeviceNameSettings.this);
				return this;
			}
		});
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double margin = Millimeter.toPixels(2.5);
		double elementsWidth = (w - margin);
		label.doLayout(0, 0, 3 * elementsWidth / 5, h);
		button.doLayout(label.getRelativePosition().getWidth() + margin, 0, 2 * elementsWidth / 5, h);
		button.applyPadding();
	}

	@Override
	public String getCurrentName() {
		return label.getLabel();
	}

	@Override
	public String getEntityName() {
		return "device";
	}

	@Override
	public void setName(String newName) {
		SystemSettings.get().setDeviceName(newName);
	}

	@Override
	public String getTitleName() {
		return getCurrentName();
	}
}
