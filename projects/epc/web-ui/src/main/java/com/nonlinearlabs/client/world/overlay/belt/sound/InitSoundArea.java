package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class InitSoundArea extends OverlayLayout {

	private SoundBeltButton init;
	private StoreInitButton store;
	private ResetInitButton reset;

	protected InitSoundArea(BeltSoundLayout parent) {
		super(parent);

		addChild(init = new InitSoundButton(this));
		addChild(store = new StoreInitButton(this));
		addChild(reset = new ResetInitButton(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonDim = Millimeter.toPixels(10);
		double margin = buttonDim / 2;
		double height = 2 * buttonDim + margin;
		double yPos = (h - height) / 2;
		init.doLayout(0, yPos, w, buttonDim);
		store.doLayout(0, yPos + buttonDim + margin, (w - margin) / 2, buttonDim);
		reset.doLayout(store.getRelativePosition().getRight() + margin, yPos + buttonDim + margin, (w - margin) / 2,
				buttonDim);
	}
}
