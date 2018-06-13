package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class Scale extends ParameterGroupVertical {

	public Scale(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, getName()) {
			@Override
			public RGB getColorFont() {
				return new RGB(190, 190, 190);
			}
		});
		addChild(new ScaleControls(this));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().fill(ctx, getColorModuleBackground());
		super.draw(ctx, invalidationMask);
	}

	@Override
	public RGB getColorSliderHighlight() {
		return new RGB(155, 159, 163);
	}

	@Override
	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	@Override
	public RGB getColorModuleBackground() {
		return ColorTable.getColorAUXBackground();
	}

	@Override
	public RGB getColorModuleHeaderBackground() {
		return new RGB(155, 159, 163);
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return new RGB(155, 159, 163);
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return new RGB(155, 159, 163);
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorAUXSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return new RGB(155, 159, 163);
	}

	@Override
	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	@Override
	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

	@Override
	public String getID() {
		return "Scale";
	}
}
