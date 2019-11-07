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
import com.nonlinearlabs.client.dataModel.editBuffer.RibbonParameterModel;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();
	private AnimationManager animationManager = new AnimationManager();

	public static EditBufferUseCases get() {
		return theInstance;
	}

	public void setParameterValue(int id, double newValue, boolean oracle) {
		setParameterValue(id, newValue, oracle, true);
	}

	private void setParameterValue(int id, double newValue, boolean oracle, boolean setAnimationTimeout) {

		BasicParameterModel p = EditBufferModel.findParameter(id);
		double oldQ = p.value.getQuantizedAndClipped(true);
		p.value.value.setValue(newValue);
		double newQ = p.value.getQuantizedAndClipped(true);
		double diff = newQ - oldQ;

		if (p instanceof PhysicalControlParameterModel) {
			PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
			applyPhysicalControlModulation(m, diff);

			if (setAnimationTimeout)
				animationManager.startDelayedAnimation(m, 2000);
			else
				animationManager.cancelAnimation(m);
		}

		if (p instanceof MacroControlParameterModel)
			applyModulationToModulateableParameters(id, diff);

		NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}

	private void startReturningAnimation(PhysicalControlParameterModel m) {
		animationManager.startReturnAnimation(m);
	}

	private void handleBidirectionalRibbonBinding(MacroControlParameterModel m) {
		for (int routerId : m.getRouterIDs()) {
			ModulationRouterParameterModel r = this.<ModulationRouterParameterModel>findParameter(routerId);

			if (r.value.getQuantized(true) > 0.0) {
				int physicalControlID = r.getAssociatedPhysicalControlID();

				int ribbon1 = 284;
				int ribbon2 = 289;

				if (physicalControlID == ribbon1 || physicalControlID == ribbon2) {
					RibbonParameterModel ribbon = this.<RibbonParameterModel>findParameter(physicalControlID);
					if (!ribbon.isReturning()) {
						ribbon.value.value.setValue(m.value.getQuantizedAndClipped(true));
					}
				}
			}
		}
	}

	private <T> T findParameter(int id) {
		return (T) EditBufferModel.findParameter(id);
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

		double oldQ = macroControl.value.getQuantizedAndClipped(true);
		double v = macroControl.value.value.getValue();
		macroControl.value.value.setValue(v + delta);
		double newQ = macroControl.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(macroControlID, newQ - oldQ);
	}

	private void applyModulationToModulateableParameters(int macroControlID, double d) {
		if (d != 0) {
			ModSource m = ModSource.fromParameterId(macroControlID);
			for (ModulateableParameterModel t : EditBufferModel.getAllModulateableParameters()) {
				if (t.modSource.getValue() == m) {
					double amount = t.modAmount.getQuantizedAndClipped(true);
					if (t.value.metaData.bipolar.getBool())
						amount *= 2;
					t.value.value.setValue(t.value.value.getValue() + d * amount);
				}
			}

			MacroControlParameterModel mc = EditBufferModel.findParameter(m);
			handleBidirectionalRibbonBinding(mc);
		}
	}

	private void applyDirectModulation(ModulationRouterParameterModel routerParameter, double newValue) {
		MacroControlParameterModel m = (MacroControlParameterModel) EditBufferModel
				.findParameter(routerParameter.getAssociatedMacroControlID());
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

	private void setModulationUpperBound(int parameterId, double newAmount, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(parameterId);
		MacroControlParameterModel mc = EditBufferModel.findParameter(p.modSource.getValue());

		double factor = p.value.metaData.bipolar.getBool() ? 2 : 1;
		double oldAmount = p.modAmount.getClippedValue();
		double oldValue = p.value.getClippedValue();
		double mcValue = mc.value.getClippedValue();
		double oldLowerBound = oldValue - (factor * oldAmount) * mcValue;
		
		setModulationAmount(parameterId, newAmount, true);

		double newLowerBound = oldValue - (factor * newAmount) * mcValue;
		double lowerBoundDiff = newLowerBound - oldLowerBound;
		setParameterValue(parameterId, oldValue - lowerBoundDiff, true);
	}

	private void setModulationLowerBound(int parameterId, double newAmount, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(parameterId);
		MacroControlParameterModel mc = EditBufferModel.findParameter(p.modSource.getValue());

		double factor = p.value.metaData.bipolar.getBool() ? 2 : 1;
		double oldAmount = p.modAmount.getClippedValue();
		double oldValue = p.value.getClippedValue();
		double mcValue = mc.value.getClippedValue();
		double oldUpperBound = oldValue + (factor * oldAmount) * (1.0 - mcValue);
		
		setModulationAmount(parameterId, newAmount, true);

		double newUpperBound = oldValue + (factor * newAmount) * (1.0 - mcValue);
		double upperBoundDiff = newUpperBound - oldUpperBound;
		setParameterValue(parameterId, oldValue - upperBoundDiff, true);
	}

	public void setModulationSource(int id, ModSource src) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		if (p.modSource.setValue(src))
			NonMaps.get().getServerProxy().setModulationSource(src);
	}

	public IncrementalChanger startEditParameterValue(int id, double pixelsPerRange) {
		BasicParameterModel p = EditBufferModel.findParameter(id);
		return new IncrementalChanger(p.value, pixelsPerRange, (v, b) -> setParameterValue(id, v, true, b), () -> {
			if (p instanceof PhysicalControlParameterModel) {
				PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
				if (m.isReturning())
					startReturningAnimation(m);
			}
		});
	}

	public IncrementalChanger startEditMCAmount(int id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationAmount(id, v, true), null);
	}

	public IncrementalChanger startEditMacroControlValue(int id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);

		if (p.modSource.getValue() != ModSource.None)
			return startEditParameterValue(p.modSource.getValue().toParameterId(), pixelsPerRange);

		return null;
	}

	public IncrementalChanger startEditModulationAmountLowerBound(int id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange,
				(v, b) -> setModulationLowerBound(id, v, true), null) {
					@Override
					public double bendAmount(double i) {
						return -i / 2;
					}
				};
	}

	public IncrementalChanger startEditModulationAmountUpperBound(int id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.findParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationUpperBound(id, v, true),
				null) {
					@Override
					public double bendAmount(double i) {
						return i / 2;
					}
				};
	}

	public void renameMacroControl(int parameterID, String newName) {
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(parameterID);
		m.givenName.fromString(newName);
		NonMaps.theMaps.getServerProxy().renameMacroControl(parameterID, newName);
	}

}
