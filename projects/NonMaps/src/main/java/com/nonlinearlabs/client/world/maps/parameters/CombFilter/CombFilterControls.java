package com.nonlinearlabs.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class CombFilterControls extends ParameterGroupControls {

	CombFilterControls(MapsLayout parent) {
		super(parent);
		addChild(new CombFilterCol1(this));
		addChild(new CombFilterCol2(this));
		addChild(new CombFilterCol3(this));
		addChild(new CombFilterCol4(this));
		addChild(new CombFilterCol5(this));
		addChild(new CombFilterCol6(this));
		addChild(new CombFilterCol7(this));
	}
}
