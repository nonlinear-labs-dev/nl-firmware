package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.HashMap;

public class ParameterGroup {
	private static ParameterFactory factory = new ParameterFactory();

	public StringDataModelEntity shortName = new StringDataModelEntity();
	public StringDataModelEntity longName = new StringDataModelEntity();

	private HashMap<Integer, BasicParameter> parameters = new HashMap<Integer, BasicParameter>();

	public BasicParameter findParameter(int id) {
		BasicParameter p = parameters.get(id);

		if (p == null)
			p = addParameter(id);

		return p;
	}

	public BasicParameter addParameter(int id) {
		BasicParameter p = factory.create(id);

		if (p != null)
			parameters.put(id, p);

		return p;
	}

}
