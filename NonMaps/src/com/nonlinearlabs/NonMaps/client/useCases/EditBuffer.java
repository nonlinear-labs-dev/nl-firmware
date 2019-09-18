package com.nonlinearlabs.NonMaps.client.useCases;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;

public class EditBuffer {
	private static EditBuffer theInstance = new EditBuffer();

	public static EditBuffer get() {
		return theInstance;
	}

	public void setParameterValue(String groupId, int id, double newValue, boolean oracle) {

		BasicParameterModel p = com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel.get()
				.findParameter(id);
		if (p != null)
			p.value.value.setValue(newValue);

		NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}

	public void selectParameter(int id) {
		if (com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel.get().selectedParameter.setValue(id))
			NonMaps.get().getServerProxy().selectParameter(id);
	}

	public void setParameterDefault(int parameterID) {
		// TODO Auto-generated method stub

	}

	public void toggleBooleanParameter(int parameterID) {
		/*-
		if (getValue().getQuantizedClipped() != 0.0)
			getValue().setRawValue(Initiator.EXPLICIT_USER_ACTION, 0.0);
		else
			getValue().setRawValue(Initiator.EXPLICIT_USER_ACTION, 1.0);-*/

	}

	public IncrementalChanger startUserEdit(int parameterID, double pixels) {
		BasicParameterModel p = com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel.get()
				.findParameter(parameterID);

		return null;
	}
}
