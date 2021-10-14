package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.maps.SVGImage;

public class ModulationRoutingButton extends SVGImage {

	ParameterPresenter presenter;

	public ModulationRoutingButton(ModulationRoutingParameter parent, int parameterId) {
		this(parent, parameterId, "Control_Through_Disabled.svg", "Control_Through_Enabled.svg",
				"Control_Through_Active.svg");
	}

	public ModulationRoutingButton(ModulationRoutingParameter parent, int parameterId, String... images) {
		super(parent, images);

		ParameterPresenterProviders.get().registerForCurrentVoiceGroup(parameterId, p -> {
			presenter = p;
			return true;
		});
	}

	@Override
	public ModulationRoutingParameter getParent() {
		return (ModulationRoutingParameter) super.getParent();
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(presenter.hash);
	}

	@Override
	public int getSelectedPhase() {
		return presenter.controlPosition != 0.0 ? 1 : 0;
	}

	@Override
	protected double getBasicWidth() {
		return 120;
	}

	@Override
	protected double getYDimension(double levelOfDetail) {
		double y = 0;
		y += calcYDimension(levelOfDetail, getLevelOfDetailForFullVisibility() + 1, 20, 10);
		y += calcYDimension(levelOfDetail, getLevelOfDetailForFullVisibility(), 9, 0);
		y += calcYDimension(levelOfDetail, 5, 19, 7.5);
		return y;
	}

	@Override
	protected double getBasicHeight() {
		return 0;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	@Override
	protected double getPadding() {
		return 4;
	}

}
