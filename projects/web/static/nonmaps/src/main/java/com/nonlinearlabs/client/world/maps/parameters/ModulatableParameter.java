package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulatableParameter extends ModulatableParameterWithoutName {

	public ModulatableParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulateableParameterName(this));
	}

	public ModulatableParameter(MapsLayout parent, String name, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulateableParameterName(this, name));
	}

}
