package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.Label;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class ModulationSourceLabel extends Label {

	public ModulationSourceLabel(MapsLayout parent) {
		super(parent, "");
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		setNonSize(0, getNonPosition().getHeight());
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
		setNonSize(parentsWidthFromFirstPass * 0.9, getNonPosition().getHeight());
	}

	@Override
	protected double getBasicHeight() {
		return 17;
	}

	@Override
	protected double getFontHeight() {
		return 10;
	}

	@Override
	protected String getDisplayText() {
		Parameter p = Parameter.findInParents(getParent());
		ModulatableParameter mp = (ModulatableParameter) p;
		return mp.getModSourceString();
	}

	@Override
	protected TextAlign getTextAlignment() {
		return TextAlign.RIGHT;
	}

	@Override
	public RGB getColorFont() {
		Parameter p = Parameter.findInParents(getParent());
		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			if (m.isSelectedParameterMyMacroControl())
				return ColorTable.getMacroControlTargetColor();
		}
		return super.getColorFont();
	}
}
