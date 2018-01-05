package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class ScaleCol1 extends ParameterColumn {

	ScaleCol1(MapsLayout parent) {
		super(parent);

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 312;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 316;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 320;
			}
		});

	}
}
