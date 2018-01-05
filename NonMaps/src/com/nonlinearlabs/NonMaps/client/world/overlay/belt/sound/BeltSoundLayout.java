package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;

public class BeltSoundLayout extends OverlayLayout {

	private TransitionTimeArea transitionTime;
	private RandomizeSoundArea randomize;

	public BeltSoundLayout(Belt belt) {
		super(belt);

		addChild(new InitSoundArea(this));
		addChild(randomize = new RandomizeSoundArea(this));
		addChild(transitionTime = new TransitionTimeArea(this));
		addChild(new ConvertSoundArea(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double margin = getButtonDimension();
		double margins = (getChildren().size() + 1) * margin;
		double widthPerElement = (w - margins) / (getChildren().size());
		double xPos = margin;
		double height = h;
		double yPos = (h - height) / 2;

		for (OverlayControl c : getChildren()) {
			c.doLayout(xPos, yPos, widthPerElement, height);
			xPos += widthPerElement + margin;
		}
	}

	public void update(Node settings) {
		transitionTime.update(settings);
		randomize.update(settings);
	}
}