package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();

	public static EditBufferUseCases get() {
		return theInstance;
	}

	public void setParameterValue(int id, double newValue, boolean oracle) {

		BasicParameterModel p = EditBufferModel.findParameter(id);
		double oldQ = p.value.getQuantizedAndClipped(true);
		p.value.value.setValue(newValue);
		double newQ = p.value.getQuantizedAndClipped(true);
		double diff = newQ - oldQ;

		if (p instanceof PhysicalControlParameterModel)
			applyPhysicalControlModulation((PhysicalControlParameterModel) p, diff);

		if (p instanceof MacroControlParameterModel)
			applyModulationToModulateableParameters(id, diff);

		NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}


	private void applyPhysicalControlModulation(PhysicalControlParameterModel p, double diff) {

		if (Math.abs(diff) > 0.0) {
			for (int router = 0; router < 4; router++) {
				int routerId = p.id + router + 1;
				ModulationRouterParameterModel routerParameter = (ModulationRouterParameterModel) EditBufferModel
						.findParameter(routerId);

				if (p.isReturning())
					applyReturningModulation(routerParameter, diff);
				else if (routerParameter.value.value.getValue() != 0) {
					applyDirectModulation(routerParameter, p.value.getQuantizedAndClipped(true));
				}
			}
		}
	}

	private void applyReturningModulation(ModulationRouterParameterModel routerParameter, double diff) {
		double amount = routerParameter.value.getQuantizedAndClipped(true);

		if (amount != 0.0) {
			int mc = routerParameter.getAssociatedMacroControlID();
			applyModulation(mc, diff * amount);
		}
	}

	private void applyModulation(int macroControlID, double delta) {
		BasicParameterModel macroControl = EditBufferModel.findParameter(macroControlID);
		if (macroControl.value.metaData.bipolar.getBool())
			delta *= 2;

		double oldQ = macroControl.value.getQuantizedAndClipped(true);
		double v = macroControl.value.value.getValue();
		macroControl.value.value.setValue(v + delta);
		double newQ = macroControl.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(macroControlID, newQ - oldQ);
	}

	private void applyModulationToModulateableParameters(int macroControlID, double d) {
		if (d != 0) {
			ModSource m = ModSource.fromParameterId(macroControlID);
			for(ModulateableParameterModel t : EditBufferModel.getAllModulateableParameters()) {
				if(t.modSource.getValue() == m) {
					double amount = t.modAmount.getQuantizedAndClipped(true);
					t.value.value.setValue(t.value.value.getValue() + d * amount);
				}
			}
		}
	}

	private void applyDirectModulation(ModulationRouterParameterModel routerParameter, double newValue) {
		MacroControlParameterModel m = (MacroControlParameterModel) EditBufferModel.findParameter(routerParameter.getAssociatedMacroControlID());
		double oldQ = m.value.getQuantizedAndClipped(true);
		m.value.value.setValue(newValue);
		double newQ = m.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(m.id, newQ - oldQ);
	}

	public void selectParameter(int id) {
		if (EditBufferModel.selectedParameter.setValue(id))
			NonMaps.get().getServerProxy().selectParameter(id);
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

	public void decModulationAmount(int id, boolean fine) {
		incDecModulationAmount(id, fine, -1);
	}

	public void incModulationAmount(int id, boolean fine) {
		incDecModulationAmount(id, fine, 1);
	}

	public void resetModulationAmount(int id) {
		setModulationAmount(id, 0, false);
	}

	private void incDecModulationAmount(int id, boolean fine, int inc) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		double v = p.modAmount.getIncDecValue(fine, inc);
		setModulationAmount(id, v, true);
	}

	public void setModulationAmount(int id, double newValue, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		double oldValue = p.modAmount.getQuantizedAndClipped(fine);
		p.modAmount.value.setValue(newValue);
		newValue = p.modAmount.getQuantizedAndClipped(fine);
		double diff = newValue - oldValue;

		if (diff != 0)
			NonMaps.get().getServerProxy().setModulationAmount(newValue);
	}

	public void setModulationSource(int id, ModSource src) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		if (p.modSource.setValue(src))
			NonMaps.get().getServerProxy().setModulationSource(src);
	}

	public IncrementalChanger startEditParameterValue(int id, double pixelsPerRange) {
		BasicParameterModel p = EditBufferModel.findParameter(id);
		return new IncrementalChanger(p.value, pixelsPerRange, v -> setParameterValue(id, v, true));
	}

	public IncrementalChanger startEditMCAmount(int id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, v -> setModulationAmount(id, v, true));
	}

	public IncrementalChanger startEditMacroControlValue(int id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);

		if (p.modSource.getValue() != ModSource.None)
			return startEditParameterValue(p.modSource.getValue().toParameterId(), pixelsPerRange);

		return null;
	}

	public IncrementalChanger startEditModulationAmountLowerBound(int id, double pixelsPerRange) {
		// TODO
		return null;
	}

	public IncrementalChanger startEditModulationAmountUpperBound(int id, double pixelsPerRange) {
		// TODO
		return null;
	}

}
