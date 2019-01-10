package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class KeyTrkLabelModulationSource extends LabelModulationSource {
	
	public KeyTrkLabelModulationSource(MapsLayout parent, Name name) {
		super(parent, name);
	}

	@Override
	protected double getBasicHeight() {
		return 15;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}
}
