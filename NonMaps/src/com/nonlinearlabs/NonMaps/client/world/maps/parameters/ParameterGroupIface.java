package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.Name;

public interface ParameterGroupIface {
	String getID();

	Name getName();

	void setName(Name name);

	boolean isLocked();
}
