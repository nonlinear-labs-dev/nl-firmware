package com.nonlinearlabs.client.world.maps.parameters.Cabinet;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class CabinetControls extends ParameterGroupControls {

	CabinetControls(MapsLayout parent) {
		super(parent);
		addChild(new DriveColumn(this));
		addChild(new DriveExtColumn(this));
		addChild(new TiltColumn(this));
		addChild(new FilterColumn(this));
		addChild(new LevelColumn(this));
		addChild(new MixColumn(this));
	}
}
