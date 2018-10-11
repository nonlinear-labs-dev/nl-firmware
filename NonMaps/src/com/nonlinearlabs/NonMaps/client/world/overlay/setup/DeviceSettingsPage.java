package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.PlayControls;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class DeviceSettingsPage extends SetupPage {
	protected DeviceSettingsPage(SetupPages parent) {
		super(parent);

		addChild(new VelocityCurveSetting(this));
		addChild(new Divider(this));
		addChild(new AftertouchCurveSetting(this));
		addChild(new Divider(this));
		addChild(new BenderCurve(this));
		addChild(new Divider(this));
		addChild(new CenteredSetupLabel(this, "Pedal Settings"));

		PlayControls playControls = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getPlayControls();

		addChild(new PedalSetting(this, playControls.getPedal(0), "Pedal 1"));
		addChild(new PedalSetting(this, playControls.getPedal(1), "Pedal 2"));
		addChild(new PedalSetting(this, playControls.getPedal(2), "Pedal 3"));
		addChild(new PedalSetting(this, playControls.getPedal(3), "Pedal 4"));

		addChild(new Divider(this));
		addChild(new PresetGlitchSetting(this));
		addChild(new Divider(this));
		addChild(new EditSmoothingTimeSetting(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double lineHeight = Millimeter.toPixels(10);
		double margin = Millimeter.toPixels(2.5);
		double yPos = 0;

		for (OverlayControl c : getChildren()) {
			c.doLayout(margin, yPos, w - 2 * margin, lineHeight);
			yPos += c.getRelativePosition().getHeight();
		}
	}

}
