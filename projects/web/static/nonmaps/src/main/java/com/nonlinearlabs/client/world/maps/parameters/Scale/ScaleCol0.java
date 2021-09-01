package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.maps.MapsLayout;

class ScaleCol0 extends ScaleCol {

	ScaleCol0(MapsLayout parent) {
		super(parent);

		addChild(new ScaleParameter(this, 312) {
			@Override
			protected void startMouseEdit() {
				currentParameterChanger = EditBufferUseCases.get().startEditParameterValue(getParameterNumber(),
				Millimeter.toPixels(100));
			}
		});
	}
}
