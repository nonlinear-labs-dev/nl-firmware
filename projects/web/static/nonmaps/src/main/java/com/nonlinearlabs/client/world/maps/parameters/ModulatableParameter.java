package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulatableParameter extends ModulatableParameterWithoutName {

	public ModulatableParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		if(parent.getChildren().size() == 0)
		{
			if(parent instanceof ParameterColumn)
			{
				ParameterColumn col = (ParameterColumn)parent;
				if(col.shouldDisplayHeaderOnFirstParameter())
				{
					addChild(new ModulateableParameterName(this));
				}
			}
		}
	}

	public ModulatableParameter(MapsLayout parent, String name, int parameterID) {
		super(parent, parameterID);
		if(parent.getChildren().size() == 0)
		{
			if(parent instanceof ParameterColumn)
			{
				ParameterColumn col = (ParameterColumn)parent;
				if(col.shouldDisplayHeaderOnFirstParameter())
				{
					addChild(new ModulateableParameterName(this, name));
				}
			}
		}
	}

}
