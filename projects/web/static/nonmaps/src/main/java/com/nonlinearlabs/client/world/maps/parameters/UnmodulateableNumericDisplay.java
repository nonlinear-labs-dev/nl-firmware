package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class UnmodulateableNumericDisplay extends ModulationSourceHighPriority {

	public UnmodulateableNumericDisplay(MapsLayout parent, int parameterID) {
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
}
