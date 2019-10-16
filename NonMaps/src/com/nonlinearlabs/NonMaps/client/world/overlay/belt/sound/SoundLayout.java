package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SoundLayout extends OverlayLayout {

	private SoundTypeMenu soundTypeMenu;
	private OverlayControl soundSettings;
	private SoundSettingsButton settings;

	protected SoundLayout(OverlayLayout parent) {
		super(parent);

		this.soundTypeMenu = addChild(new SoundTypeMenu(this));
		this.settings = addChild(new SoundSettingsButton(this));
	}

	protected void setSettings(OverlayLayout s) {
		this.soundSettings = addChild(s);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		int parts = 8;
		double partSize = w / parts;

		double menuWidth = soundTypeMenu.getPictureWidth();
		double menuHeight = soundTypeMenu.getPictureHeight();
		soundTypeMenu.doLayout(1 * partSize - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight);
		soundSettings.doLayout(2 * partSize, 0, 4 * partSize, h);
		double settingsWidth = settings.getPictureWidth();
		double settingsHeight = settings.getPictureHeight();
		settings.doLayout(7 * partSize - settingsWidth / 2, h / 2 - settingsHeight / 2, settingsWidth, settingsHeight);
	}
}
