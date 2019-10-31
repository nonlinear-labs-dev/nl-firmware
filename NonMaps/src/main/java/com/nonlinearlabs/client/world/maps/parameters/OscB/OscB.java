package com.nonlinearlabs.client.world.maps.parameters.OscB;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;

public class OscB extends ParameterGroup {

	public OscB(MapsLayout parent) {
		super(parent, "Osc B");
		addChild(new LabelModuleHeader(this));
		addChild(new OscBControls(this));
	}

	public RGB getColorSliderHighlight() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	public RGB getColorModuleBackground() {
		return ColorTable.getColorEnvOscBBackground();
	}

	public RGB getColorModuleHeaderBackground() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorEnvOscBSelectedBackground();
	}

	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorEnvOscBHeaderBackground();
	}

	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

}
