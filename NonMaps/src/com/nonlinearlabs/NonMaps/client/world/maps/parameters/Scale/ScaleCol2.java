package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class ScaleCol2 extends ParameterColumn {

	ScaleCol2(MapsLayout parent) {
		super(parent);
		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 313;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 317;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 321;
			}
		});
	}
}
