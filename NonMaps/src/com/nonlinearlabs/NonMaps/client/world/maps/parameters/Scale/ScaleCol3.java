package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

class ScaleCol3 extends ScaleCol {

	ScaleCol3(MapsLayout parent) {
		super(parent);
		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 314;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 318;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 322;
			}
		});
	}
}
