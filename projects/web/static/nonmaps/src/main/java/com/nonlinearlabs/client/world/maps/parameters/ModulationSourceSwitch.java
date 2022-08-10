package com.nonlinearlabs.client.world.maps.parameters;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;

public class ModulationSourceSwitch extends ModulationSourceHighPriority {

	public ModulationSourceSwitch(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelModulationSource(this, getParameterNumber()));

		addChild(new KeyTrackParameter(this, getParameterNumber()));

		addChild(new Spacer(this, 1, 10) {
			@Override
			protected double getMinHeight() {
				return 7.5;
			}
		});
	}

    @Override
    protected void startMouseEdit() {
        currentParameterChanger = EditBufferUseCases.get().startEditParameterValue(getParameterNumber(),
                getPixRect().getWidth() / 2);
    }
}
