package com.nonlinearlabs.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;

public class EnvA extends ParameterGroup {

	public EnvA(MapsLayout parent) {
		super(parent, "Env A");
		addChild(new LabelModuleHeader(this));
		addChild(new EnvAControls(this));
	}

	public RGB getColorSliderHighlight() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	public RGB getColorModuleBackground() {
		return ColorTable.getColorEnvOscABackground();
	}

	public RGB getColorModuleHeaderBackground() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorModuleHeaderFont() {
		return ColorTable.getColorEnvOscAHeaderFont();
	}

	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorEnvOscASelectedBackground();
	}

	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorEnvOscAHeaderBackground();
	}

	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}
}
