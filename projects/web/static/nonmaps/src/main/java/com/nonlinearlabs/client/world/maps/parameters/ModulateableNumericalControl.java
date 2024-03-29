package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulateableNumericalControl extends LayoutResizingVertical {

	NumericalControlLarge numericalControl;
	NumericalControlsModulation modControls;

	public ModulateableNumericalControl(MapsLayout parent, int parameterID) {
		super(parent);

		addChild(new ModulationSourceLabel(this, parameterID));

		numericalControl = addChild(new NumericalControlLarge(this, parameterID) {
			@Override
			protected double getInsetY() {
				return 0;
			}

			@Override
			protected double getBasicHeight() {
				return 35;
			}

			@Override
			public RGB getColorSliderBackground() {
				return new RGBA(0, 0, 0, 0.0);
			}

			@Override
			public RGB getColorObjectContour() {
				return new RGBA(0, 0, 0, 0.0);
			}

			@Override
			public double getLevelOfDetailForFullVisibility() {
				return getParent().getLevelOfDetailForFullVisibility();
			}

		});

		modControls = addChild(new NumericalControlsModulation(this, parameterID) {
			@Override
			public double getLevelOfDetailForFullVisibility() {
				return getParent().getLevelOfDetailForFullVisibility();
			}

			@Override
			protected double getBasicHeight() {
				return 12;
			}
			
			@Override
			public double getTopMargin() {
				return 0;
			}

			@Override
			public double getBottomMargin() {
				return 0;
			}
		});

		addChild(new Spacer(this, 80, 9));

	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		Rect pixRect = getPixRect().copy();

		pixRect.reduceWidthBy(toXPixels(10));
		pixRect.setTop(numericalControl.getPixRect().getTop());
		pixRect.setBottom(modControls.getPixRect().getBottom());
		pixRect.fillAndStroke(ctx, getColorSliderBackground(), toXPixels(1), getColorObjectContour());

		super.draw(ctx, invalidationMask);
	}

	public double getXMargin() {
		return 0;
	}

	public double getYMargin() {
		return 0;
	}

	@Override
	public double getTopMargin() {
		return 0;
	}

	@Override
	public double getBottomMargin() {
		return 0;
	}
}
