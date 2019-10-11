package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SingleSoundLayout extends OverlayLayout {

	private SoundTypeMenu soundTypeMenu;
	private SingleSoundMasterSettings masterSettings;
	private SingleSoundSettingsButton settings;

	protected SingleSoundLayout(OverlayLayout parent) {
		super(parent);

		soundTypeMenu = addChild(new SoundTypeMenu(this));
		masterSettings = addChild(new SingleSoundMasterSettings(this));
		settings = addChild(new SingleSoundSettingsButton(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		int parts = 8;
		double partSize = w / parts;

		double menuWidth = soundTypeMenu.getPictureWidth();
		double menuHeight = soundTypeMenu.getPictureHeight();
		soundTypeMenu.doLayout(1 * partSize - menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight);

		masterSettings.doLayout(2 * partSize, 4 * partSize, h);

		double settingsWidth = settings.getPictureWidth();
		double settingsHeight = settings.getPictureHeight();
		settings.doLayout(7 * partSize - settingsWidth / 2, h / 2 - settingsHeight / 2, settingsWidth, settingsHeight);
	}
}
