package com.nonlinearlabs.NonMaps.client.useCases;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;

public class EditBuffer {
	private static EditBuffer theInstance = new EditBuffer();

	public static EditBuffer get() {
		return theInstance;
	}

	public void setParameterValue(int id, double newValue, boolean oracle) {

		BasicParameterModel p = EditBufferModel.get().findParameter(id);
		if (p != null)
			p.value.value.setValue(newValue);

		NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}

	public void selectParameter(int id) {
		if (EditBufferModel.get().selectedParameter.setValue(id))
			NonMaps.get().getServerProxy().selectParameter(id);
	}

	

	public IncrementalChanger startUserEdit(int parameterID, double pixels) {
		BasicParameterModel p = EditBufferModel.get().findParameter(parameterID);
		return new IncrementalChanger(p.value, pixels);
	}

	public void incParameter(int id, Initiator initiator, boolean fine) {
		incDecParameter(id, initiator, fine, 1);
	}

	public void decParameter(int id, Initiator initiator, boolean fine) {
		incDecParameter(id, initiator, fine, -1);
	}

	private void incDecParameter(int id, Initiator initiator, boolean fine, int inc) {
		BasicParameterModel p = EditBufferModel.get().findParameter(id);
		double v = p.getIncDecValue(fine, inc);
		setParameterValue(id, v, true);
	}
}
