package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class FBMixerControls extends ParameterGroupControls {

	FBMixerControls(MapsLayout parent) {
		super(parent);
		addChild(new CombColumn(this));
		addChild(new SVFilterColumn(this));
		addChild(new EffectsColumn(this));
		addChild(new ReverbColumn(this));
		addChild(new DriveColumn(this));
		addChild(new KeyTrackColumn(this));
	}
}
