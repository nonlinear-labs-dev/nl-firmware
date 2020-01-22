package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;

public class CombFilter extends ParameterGroup {

	public CombFilter(MapsLayout parent) {
		super(parent, "Comb");
		addChild(new LabelModuleHeader(this));
		addChild(new CombFilterControls(this));
	}

	@Override
	public RGB getColorSliderHighlight() {
		return ColorTable.getColorCombHeaderBackground();
	}

	@Override
	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	@Override
	public RGB getColorModuleBackground() {
		return ColorTable.getColorCombBackground();
	}

	@Override
	public RGB getColorModuleHeaderBackground() {
		return ColorTable.getColorCombHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorCombHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorCombHeaderBackground();
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorCombSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorCombHeaderBackground();
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
