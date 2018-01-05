package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscB;

import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class OscB extends ParameterGroupVertical {

	public OscB(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, getName()));
		addChild(new OscBControls(this));
	}

	@Override
	public String getID() {
		return "Osc B";
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
