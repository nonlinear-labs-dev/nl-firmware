package com.nonlinearlabs.NonMaps.client.useCases;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.BasicParameter;
import com.nonlinearlabs.NonMaps.client.dataModel.ParameterGroup;

public class EditBuffer {
	private static EditBuffer theInstance = new EditBuffer();

	public static EditBuffer get() {
		return theInstance;
	}

	public void setParameterValue(String groupId, int id, double newValue, boolean oracle) {
		ParameterGroup g = com.nonlinearlabs.NonMaps.client.dataModel.EditBuffer.get().getGroup(groupId);

		BasicParameter p = g.findParameter(id);
		if (p != null)
			p.value.setValue(newValue);

		NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}
}
