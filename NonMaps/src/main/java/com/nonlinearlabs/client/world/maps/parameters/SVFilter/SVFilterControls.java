package com.nonlinearlabs.client.world.maps.parameters.SVFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class SVFilterControls extends ParameterGroupControls {

	SVFilterControls(MapsLayout parent) {
		super(parent);
		addChild(new ABColumn(this));
		addChild(new CombMixColumn(this));
		addChild(new CutOffColumn(this));
		addChild(new ResoColumn(this));
		addChild(new SpreadColumn(this));
		addChild(new LBHColumn(this));
		addChild(new FMColumn(this));
	}
}
