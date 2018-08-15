package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class Scale extends ParameterGroupVertical {

	private ScaleControls m_scaleControls;

	public Scale(MapsLayout parent) {
		super(parent);
		addChild(new ScaleLabelModuleHeader(this, getName()) {
			@Override
			public RGB getColorFont() {
				return RGB.lighterGray();
			}
		});
		m_scaleControls = addChild(new ScaleControls(this));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().fill(ctx, getColorModuleBackground());
		super.draw(ctx, invalidationMask);
	}

	@Override
	public RGB getColorSliderHighlight() {
		return RGB.lightGray();
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
		return RGB.lightGray();
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorAUXSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	@Override
	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

	public boolean anyValueNotDefault() {
		boolean notDefault = false;
		for (ScaleParameter param : m_scaleControls.getParameters()) {
			if (param.getParameterID() != 312) {
				double value = param.getValue().getRawValue();
				double defaultValue = param.getDefaultValue();
				notDefault |= value != defaultValue;
			}
		}
		return notDefault;
	}

	@Override
	public String getID() {
		return "Scale";
	}
}
