package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class PedalSliderWithValue extends OverlayLayout {

	private PedalSlider slider;
	private SetupLabel label;

	protected PedalSliderWithValue(PedalSettings parent) {
		super(parent);
		addChild(label = new CenteredSetupLabel(this, "") {

			@Override
			public String getDrawText(Context2d ctx) {
				return getPedal().getDecoratedValue(true);
			}
		});
		addChild(slider = new PedalSlider(this));
	}

	@Override
	public PedalSettings getParent() {
		return (PedalSettings) super.getParent();
	}

	public Parameter getPedal() {
		return getParent().getPedal();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double sliderHeight = Millimeter.toPixels(1.5);
		label.doLayout(0, 0, w, h / 2);
		slider.doLayout(0, label.getRelativePosition().getHeight(), w, sliderHeight);
	}

}
