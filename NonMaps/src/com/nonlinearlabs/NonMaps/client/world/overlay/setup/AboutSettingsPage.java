package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.Millimeter;

public class AboutSettingsPage extends SetupPage {

	private AboutList list;

	protected AboutSettingsPage(SetupPages parent) {
		super(parent);

		addChild(list = new AboutList(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double margin = Millimeter.toPixels(2.5);
		list.doLayout(margin, margin, w - 2 * margin, h - 2 * margin);
	}
}
