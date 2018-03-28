package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls;

import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;

public class PlayControls extends LayoutResizingHorizontal {

	SourcesAmountsAndMacros sourcesAmountsAndMacros;

	public PlayControls(MapsLayout parent) {
		super(parent);

		sourcesAmountsAndMacros = addChild(new SourcesAmountsAndMacros(this));
	}

	@Override
	public RGB getColorSliderHighlight() {
		return ColorTable.getColorAUXHeaderBackground();
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
		return ColorTable.getColorAUXHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return ColorTable.getColorAUXHeaderBackground();
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return ColorTable.getColorAUXHeaderBackground();
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorAUXSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return ColorTable.getColorAUXHeaderBackground();
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