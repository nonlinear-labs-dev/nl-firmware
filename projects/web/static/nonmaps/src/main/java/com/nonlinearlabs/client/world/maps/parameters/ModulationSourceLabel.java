package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.Label;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulationSourceLabel extends Label {

	ParameterPresenter presenter;

	public ModulationSourceLabel(MapsLayout parent, int parameterID) {
		super(parent, "");
		ParameterPresenterProviders.get().registerForCurrentVoiceGroup(parameterID, p -> {
			presenter = p;
			return true;
		});
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		setNonSize(0, getNonPosition().getHeight());
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
		double inset = (getBasicHeight() - getFontHeight()) / 2 + moveFontVerticallyBy() * 2;
		setNonSize(parentsWidthFromFirstPass - inset, getNonPosition().getHeight());
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
		return presenter.modulation.modulationSourceLabel;
	}

	@Override
	protected TextAlign getTextAlignment() {
		return TextAlign.RIGHT;
	}

	@Override
	protected double moveFontVerticallyBy() {
		return super.moveFontVerticallyBy() + 1;
	}

	@Override
	public RGB getColorFont() {
		Parameter p = Parameter.findInParents(getParent());
		if (p instanceof ModulatableParameterWithoutName) {
			ModulatableParameterWithoutName m = (ModulatableParameterWithoutName) p;
			if (m.isSelectedParameterMyMacroControl())
			{
				return ColorTable.getMacroControlTargetColor();
			}
		}
		return super.getColorFont();
	}

}
