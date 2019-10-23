package com.nonlinearlabs.client.world.maps.parameters.Scale;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

public class ScaleParameter extends ModulationSourceHighPriority {

	public ScaleParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
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