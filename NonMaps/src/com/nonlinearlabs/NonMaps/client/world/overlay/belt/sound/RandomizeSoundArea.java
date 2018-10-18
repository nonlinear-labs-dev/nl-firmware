package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class RandomizeSoundArea extends OverlayLayout {

	private RandomLevelValueEdit randomValue;
	private RandomizeSoundButton button;

	public RandomizeSoundArea(BeltSoundLayout parent) {
		super(parent);

		addChild(button = new RandomizeSoundButton(this));
		addChild(randomValue = new RandomLevelValueEdit(this));
	}

	public void update(Node settings) {
		randomValue.update(settings);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonDim = Millimeter.toPixels(10);
		double margin = buttonDim / 2;
		double height = 2 * buttonDim + margin;
		double yPos = (h - height) / 2;
		button.doLayout(0, yPos, w, buttonDim);
		randomValue.doLayout(0, yPos + buttonDim + margin, w, buttonDim);
	}
}
