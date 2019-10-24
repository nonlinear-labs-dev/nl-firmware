package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();

	public static EditBufferUseCases get() {
		return theInstance;
	}

	public void setParameterValue(int id, double newValue, boolean oracle) {

		BasicParameterModel p = EditBufferModel.findParameter(id);
		double oldValue = p.value.getQuantizedAndClipped(true);
		p.value.value.setValue(newValue);
		newValue = p.value.getQuantizedAndClipped(true);
		double diff = newValue - oldValue;

		if (p instanceof PhysicalControlParameterModel)
			applyPhysicalControlModulation((PhysicalControlParameterModel) p, diff);

		// if (p instanceof ModulationRouterParameterModel)
		// applyPhysicalControlModulation(id, diff);

		if (p instanceof MacroControlParameterModel)
			applyMacroControlModulation(id, diff);

		NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}

	private void applyMacroControlModulation(int id, double diff) {
	}

	private void applyPhysicalControlModulation(PhysicalControlParameterModel p, double diff) {

		if (Math.abs(diff) > 0.0) {
			for (int router = 0; router < 4; router++) {
				int routerId = p.id + router + 1;
				ModulationRouterParameterModel routerParameter = (ModulationRouterParameterModel) EditBufferModel
						.findParameter(routerId);

				if (p.isReturning())
					applyReturningModulation(routerParameter, diff);
				else
					applyDirectModulation(routerParameter, p.value.getQuantizedAndClipped(true));
			}
		}
	}

	private void applyDirectModulation(ModulationRouterParameterModel routerParameter, double newValue) {
		setParameterValue(routerParameter.getAssociatedMacroControlID(), newValue, true);
	}

	private void applyReturningModulation(ModulationRouterParameterModel routerParameter, double diff) {
		double amount = routerParameter.value.getQuantizedAndClipped(true);

		if (amount != 0.0)
			applyModulation(routerParameter.getAssociatedMacroControlID(), diff * amount);
	}

	private void applyModulation(int macroControlID, double delta) {
		BasicParameterModel macroControl = EditBufferModel.findParameter(macroControlID);
		if (macroControl.value.metaData.bipolar.getBool())
			delta *= 2;
		double v = macroControl.value.value.getValue();
		macroControl.value.value.setValue(v + delta);
	}

	public void selectParameter(int id) {
		if (EditBufferModel.selectedParameter.setValue(id))
			NonMaps.get().getServerProxy().selectParameter(id);
	}

	public IncrementalChanger startUserEdit(int parameterID, double pixels) {
		BasicParameterModel p = EditBufferModel.findParameter(parameterID);
		return new IncrementalChanger(p, pixels);
	}

	public void incParameter(int id, boolean fine) {
		incDecParameter(id, fine, 1);
	}

	public void decParameter(int id, boolean fine) {
		incDecParameter(id, fine, -1);
	}

	private void incDecParameter(int id, boolean fine, int inc) {
		BasicParameterModel p = EditBufferModel.findParameter(id);
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
		EditBufferModel.soundType.setValue(SoundType.Single);
	}

	public void convertToSplitSound() {
		// TODO: connect ServerProxy
		EditBufferModel.soundType.setValue(SoundType.Split);
	}

	public void convertToLayerSound() {
		// TODO: connect ServerProxy
		EditBufferModel.soundType.setValue(SoundType.Layer);
	}

	public void setToDefault(int parameterID) {
		BasicParameterModel p = EditBufferModel.findParameter(parameterID);
		double v = p.value.metaData.defaultValue.getValue();
		setParameterValue(parameterID, v, true);
	}

	public void toggleBoolean(int parameterID) {
		BasicParameterModel p = EditBufferModel.findParameter(parameterID);

		if (p.value.getQuantizedAndClipped(true) != 0.0)
			setParameterValue(parameterID, 0, true);
		else
			setParameterValue(parameterID, 1, true);
	}
}
