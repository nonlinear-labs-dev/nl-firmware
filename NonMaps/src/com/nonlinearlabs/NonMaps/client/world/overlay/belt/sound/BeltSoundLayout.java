package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.Belt;

import java.util.concurrent.locks.Lock;

public class BeltSoundLayout extends OverlayLayout {

	private TransitionTimeArea transitionTime;
	private RandomizeSoundArea randomize;

	public BeltSoundLayout(Belt belt) {
		super(belt);

		addChild(new LockSymbol(this));
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
			if(c instanceof LockSymbol) {
				c.doLayout(5, 5, 30, 40);
				continue;
			}
			c.doLayout(xPos, yPos, widthPerElement, height);
			xPos += widthPerElement + margin;
		}
	}

	public void update(Node settings) {
		transitionTime.update(settings);
		randomize.update(settings);
	}

	private class LockSymbol extends Label {

		public LockSymbol(OverlayLayout parent) {
			super(parent);
			setFontHeightInMM(7);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			if(NonMaps.get().getNonLinearWorld().getParameterEditor().isAnyParameterLocked())
				return "\ue20a";
			return "";
		}
	}
}