package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

class KeyTrackColumn extends ParameterColumn {

	private class KeyTrack extends ModulationSourceHighPriority {

		private KeyTrack(MapsLayout parent) {
			super(parent, 168);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}
	}

	KeyTrackColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 299));
		addChild(new KeyTrack(this));
	}
}
