package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulationSourceSwitch extends ModulationSourceHighPriority {

	public ModulationSourceSwitch(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelModulationSource(this, getParameterNumber()) {
			@Override
			protected double getMinHeight() {
				return 0;
			}
		});
		addChild(new KeyTrackParameter(this, getParameterNumber()));
	}

	@Override
	protected void startMouseEdit() {
		currentParameterChanger = EditBufferUseCases.get().startEditParameterValue(getParameterNumber(),
				getPixRect().getWidth() / 2);
	}

	@Override
	public double getMinHeight() {
		return 15;
	}
}
