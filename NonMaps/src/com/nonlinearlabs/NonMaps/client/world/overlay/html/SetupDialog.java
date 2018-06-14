package com.nonlinearlabs.NonMaps.client.world.overlay.html;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.overlay.GWTDialog;

public class SetupDialog extends GWTDialog {

	static int lastPopupLeft = -1;
	static int lastPopupTop = -1;

	public SetupDialog() {
		super();
		add(new Setup());
		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);
	}

	public static void create() {
		SetupDialog b = new SetupDialog();
		b.setModal(false);
		b.addHeader("Setup");
		b.initalShow();
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
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}
}
