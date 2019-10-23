package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.Name;

public interface ParameterGroupIface {
	String getID();

	Name getName();

	void setName(Name name);

	boolean isLocked();
}
