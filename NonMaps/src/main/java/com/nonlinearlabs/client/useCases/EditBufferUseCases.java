package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();

	public static EditBufferUseCases get() {
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

	public void incParameter(int id, boolean fine) {
		incDecParameter(id, fine, 1);
	}

	public void decParameter(int id, boolean fine) {
		incDecParameter(id, fine, -1);
	}

	private void incDecParameter(int id, boolean fine, int inc) {
		BasicParameterModel p = EditBufferModel.get().findParameter(id);
		double v = p.getIncDecValue(fine, inc);
		setParameterValue(id, v, true);
	}

	public void initializeSound() {
		NonMaps.get().getServerProxy().initSound();
	}

	public void randomizeSound() {
		NonMaps.get().getServerProxy().randomizeSound();
	}

	public void convertToSingleSound() {
		// TODO: connect ServerProxy
		EditBufferModel.get().soundType.setValue(SoundType.Single);
	}

	public void convertToSplitSound() {
		// TODO: connect ServerProxy
		EditBufferModel.get().soundType.setValue(SoundType.Split);
	}

	public void convertToLayerSound() {
		// TODO: connect ServerProxy
		EditBufferModel.get().soundType.setValue(SoundType.Layer);
	}

	public void setToDefault(int parameterID) {
		BasicParameterModel p = EditBufferModel.get().findParameter(parameterID);
		double v = p.value.metaData.defaultValue.getValue();
		setParameterValue(parameterID, v, true);
	}

	public void toggleBoolean(int parameterID) {
		BasicParameterModel p = EditBufferModel.get().findParameter(parameterID);

		if (p.value.getQuantizedAndClipped(true) != 0.0)
			setParameterValue(parameterID, 0, true);
		else
			setParameterValue(parameterID, 1, true);
	}
}
