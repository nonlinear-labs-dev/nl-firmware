package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class TransitionTimeArea extends OverlayLayout {

	private TransitionTimeLabel label;
	private TransitionTimeValueEdit edit;

	public TransitionTimeArea(BeltSoundLayout parent) {
		super(parent);

		addChild(label = new TransitionTimeLabel(this));
		addChild(edit = new TransitionTimeValueEdit(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonDim = Millimeter.toPixels(10);
		double margin = buttonDim / 2;
		double height = 2 * buttonDim + margin;
		double yPos = (h - height) / 2;
		label.doLayout(0, yPos, w, buttonDim);
		edit.doLayout(0, yPos + buttonDim + margin, w, buttonDim);
	}

	public void update(Node settings) {
		edit.update(settings);
	}
}
