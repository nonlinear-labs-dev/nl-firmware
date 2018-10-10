package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.LockSymbol;

public class BeltSoundLayout extends OverlayLayout {

	private TransitionTimeArea transitionTime;
	private RandomizeSoundArea randomize;
	private LockSymbol lock;

	public BeltSoundLayout(Belt belt) {
		super(belt);

		addChild(new InitSoundArea(this));
		addChild(randomize = new RandomizeSoundArea(this));
		addChild(transitionTime = new TransitionTimeArea(this));
		addChild(lock = new LockSymbol(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {

		super.doLayout(x, y, w, h);

		int numAreas = 3;

		double margin = Millimeter.toPixels(2.5);
		double margins = (numAreas + 1) * margin;

		double widthPerArea = (w - margins) / (numAreas);
		double maxWidthPerArea = Millimeter.toPixels(90);

		double xPos = margin;

		if (widthPerArea > maxWidthPerArea) {
			double rest = numAreas * (widthPerArea - maxWidthPerArea);
			widthPerArea = maxWidthPerArea;
			xPos += rest / 2;
		}

		double yPos = 0;

		for (OverlayControl c : getChildren()) {
			c.doLayout(xPos, yPos, widthPerArea, h);
			xPos += widthPerArea + margin;
		}

		double lockSymbolSizeInPixel = Millimeter.toPixels(lock.getLockSizeInMM() + 1);
		lock.doLayout(w - lockSymbolSizeInPixel * 1.25, lockSymbolSizeInPixel * 0.25, lockSymbolSizeInPixel,
				lockSymbolSizeInPixel);
	}

	public void update(Node settings) {
		transitionTime.update(settings);
		randomize.update(settings);
	}
}