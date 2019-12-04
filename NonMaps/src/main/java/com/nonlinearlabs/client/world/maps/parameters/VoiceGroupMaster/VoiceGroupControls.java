package com.nonlinearlabs.client.world.maps.parameters.VoiceGroupMaster;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class VoiceGroupMasterControls extends ParameterGroupControls {

	VoiceGroupMasterControls(MapsLayout parent) {
		super(parent);
		addChild(new VoiceGroupMasterColumn(this));
	}
}
