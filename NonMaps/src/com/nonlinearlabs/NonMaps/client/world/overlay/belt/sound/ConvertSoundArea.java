package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class ConvertSoundArea extends OverlayLayout {

	private Label label;
	private MorphButton morph;
	private DualButton dual;

	public ConvertSoundArea(BeltSoundLayout parent) {
		super(parent);

		addChild(label = new Label(this) {

			@Override
			public String getDrawText(Context2d ctx) {
				return "Convert to";
			}
		});

		addChild(morph = new MorphButton(this));
		addChild(dual = new DualButton(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonDim = Millimeter.toPixels(10);
		double margin = buttonDim / 2;
		double height = 2 * buttonDim + margin;
		double yPos = (h - height) / 2;
		label.doLayout(0, yPos, w, buttonDim);
		morph.doLayout(0, yPos + buttonDim + margin, (w - margin) / 2, buttonDim);
		dual.doLayout(morph.getRelativePosition().getRight() + margin, yPos + buttonDim + margin, (w - margin) / 2,
				buttonDim);
	}

}
