package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class ScaleCol3 extends ParameterColumn {

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
