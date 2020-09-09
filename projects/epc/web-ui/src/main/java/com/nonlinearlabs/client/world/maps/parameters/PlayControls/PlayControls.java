package com.nonlinearlabs.client.world.maps.parameters.PlayControls;

import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.LayoutResizingHorizontal;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;

public class PlayControls extends LayoutResizingHorizontal {

	SourcesAmountsAndMacros sourcesAmountsAndMacros;

	public PlayControls(MapsLayout parent) {
		super(parent);

		sourcesAmountsAndMacros = addChild(new SourcesAmountsAndMacros(this));
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

	public Macros getMacroControls() {
		return sourcesAmountsAndMacros.getMacros();
	}

	public Pedal getPedal(int i) {
		return sourcesAmountsAndMacros.getPedal(i);
	}
}