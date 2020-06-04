package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class OutputMixerControls extends ParameterGroupControls {

	OutputMixerControls(MapsLayout parent) {
		super(parent);
		addChild(new AColumn(this));
		addChild(new BColumn(this));
		addChild(new CombColumn(this));
		addChild(new SVFilterColumn(this));
		addChild(new DriveColumn(this));
		addChild(new FoldAsymColumn(this));
		addChild(new LevelColumn(this));
	}
}
