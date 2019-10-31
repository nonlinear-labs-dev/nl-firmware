package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;

public class SVFilter extends ParameterGroup {

	public SVFilter(MapsLayout parent) {
		super(parent, "SVF");
		addChild(new LabelModuleHeader(this));
		addChild(new SVFilterControls(this));
	}

	@Override
	public RGB getColorSliderHighlight() {
		return ColorTable.getColorSVFHeaderBackground();
	}

	@Override
	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	@Override
	public RGB getColorModuleBackground() {
		return ColorTable.getColorSVFBackground();
	}

	@Override
	public RGB getColorModuleHeaderBackground() {
		return ColorTable.getColorSVFHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorSVFHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorSVFHeaderBackground();
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorSVFSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorSVFHeaderBackground();
	}

	@Override
	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	@Override
	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

}
