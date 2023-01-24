package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.belt.Belt.BeltTab;

public class SoundLayout extends OverlayLayout {

	private SoundTypeMenu soundTypeMenu;
	private OverlayControl soundSettings;
	protected SoundSettingsButton settings;
	private PartFade fade;

	private class PartFade extends SVGImage {

		public PartFade(Control parent) {
			super(parent, "Fade-Edit-Btn_Active.svg", "Fade-Edit-Btn_Enabled.svg", "Fade-Edit-Btn_Disabled.svg");
		}

		@Override
		public int getSelectedPhase() {
			return 1;
		}

		@Override
		public Control click(Position p) {
			NonMaps.get().getNonLinearWorld().getViewport().getOverlay().getBelt().openTab(BeltTab.FadeEditor);
			return this;
		}
	}

	protected SoundLayout(OverlayLayout parent) {
		super(parent);

		this.soundTypeMenu = addChild(new SoundTypeMenu(this));
		this.settings = addChild(new SoundSettingsButton(this));
		this.fade = addChild(new PartFade(this));
	
		EditBufferModel.get().soundType.onChange(type -> {
			fade.setVisible(type == SoundType.Layer || type == SoundType.Split);
			return true;
		});
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
		soundTypeMenu.doLayout(menuWidth / 2, h / 2 - menuHeight / 2, menuWidth, menuHeight);
		soundSettings.doLayout(2 * partSize, 0, 4 * partSize, h);

		double settingsWidth = settings.getPictureWidth();
		double settingsHeight = settings.getPictureHeight();
		double rightEnd = w - (menuWidth / 2);
		double margin = settingsWidth * 0.25;
		settings.doLayout(rightEnd - margin - settingsWidth * 2, h / 2 - settingsHeight / 2, settingsWidth, settingsHeight);
		fade.doLayout(rightEnd - settingsWidth + margin, h / 2 - settingsHeight / 2, settingsWidth, settingsHeight);
	}
}
