package com.nonlinearlabs.client.world.overlay.html.soundSettings;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.overlay.GWTDialog;

public class SoundSettingsDialog extends GWTDialog {

	static int lastPopupLeft = -1;
	static int lastPopupTop = -1;
	static SoundSettingsDialog theSetup;

	private SoundSettingsDialog() {
		super();
		add(new SoundSettings());
		getElement().addClassName("setup-dialog");
		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);
	}

	public static void showDialog() {
		if (theSetup == null) {
			theSetup = new SoundSettingsDialog();
			theSetup.setModal(false);
			theSetup.addHeader("Settings");
			theSetup.initalShow();
		}
	}

	public static void hideDialog() {
		if (theSetup != null) {
			theSetup.commit();
		}
	}

	public static void toggle() {
		if (theSetup != null)
			hideDialog();
		else
			showDialog();
	}

	public static boolean isShown() {
		return theSetup != null;
	}

	@Override
	protected void setLastPopupPos(int popupLeft, int popupTop) {
		lastPopupLeft = popupLeft;
		lastPopupTop = popupTop;
	}

	@Override
	protected int getLastPopupPosTop() {
		return lastPopupTop;
	}

	@Override
	protected int getLastPopupPosLeft() {
		return lastPopupLeft;
	}

	@Override
	protected void commit() {
		hide();
		theSetup = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}
}
