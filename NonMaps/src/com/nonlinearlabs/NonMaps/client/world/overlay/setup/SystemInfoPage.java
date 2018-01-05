package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class SystemInfoPage extends SetupPage {

	protected SystemInfoPage(SetupPages parent) {
		super(parent);

		addChild(new DeviceNameSetting(this));
		addChild(new Divider(this));
		addChild(new FreeInternalStorage(this));
		addChild(new Divider(this));
		addChild(new UISoftwareVersion(this));
		addChild(new RTSoftwareVersion(this));
		addChild(new OSVersion(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double lineHeight = getButtonDimension();
		double margin = getButtonDimension() / 2;
		double yPos = 0;

		for (OverlayControl c : getChildren()) {
			c.doLayout(margin, yPos, w - 2 * margin, lineHeight);
			yPos += c.getRelativePosition().getHeight();
		}
	}
}
