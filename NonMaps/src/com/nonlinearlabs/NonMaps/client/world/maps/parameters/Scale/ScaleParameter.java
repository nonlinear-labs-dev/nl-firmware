package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.*;

abstract public class ScaleParameter extends ModulationSourceHighPriority {

	public ScaleParameter(MapsLayout parent) {
		super(parent);
		addChild(new LabelModulationSource(this, getName()) {

			@Override
			protected String getDisplayText() {
				return getStringName();
			}
		});
		addChild(new SliderHorizontal(this));
		addChild(new ValueDisplaySmall(this));
	}

	public double getDefaultValue() {
		return 0.0;
	}

	public String getStringName() {
		return super.getName().getLongName();
	}

}