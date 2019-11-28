package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.RibbonParameterModel;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();
	private AnimationManager animationManager = new AnimationManager();

	public static EditBufferUseCases get() {
		return theInstance;
	}

	public void setParameterValue(ParameterId id, double newValue, boolean oracle) {
		setParameterValue(id, newValue, oracle, true);
	}

	private void setParameterValue(ParameterId id, double newValue, boolean oracle, boolean setAnimationTimeout) {
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
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
		for (ParameterId routerId : m.getRouterIDs()) {
			ModulationRouterParameterModel r = this.<ModulationRouterParameterModel>findParameter(routerId);

			if (r.value.getQuantized(true) > 0.0) {
				ParameterId physicalControlID = r.getAssociatedPhysicalControlID();

				int ribbon1 = 284;
				int ribbon2 = 289;

				if (physicalControlID.getNumber() == ribbon1 || physicalControlID.getNumber() == ribbon2) {
					RibbonParameterModel ribbon = this.<RibbonParameterModel>findParameter(
							new ParameterId(physicalControlID.getNumber(), VoiceGroup.Global));
					if (!ribbon.isReturning()) {
						ribbon.value.value.setValue(m.value.getQuantizedAndClipped(true));
					}
				}
			}
		}
	}

	private <T> T findParameter(ParameterId id) {
		return (T) EditBufferModel.get().getParameter(id);
	}

	private void applyPhysicalControlModulation(PhysicalControlParameterModel p, double diff) {
		if (Math.abs(diff) > 0.0) {
			// TODO: which voice group? Both?
			for (int router = 0; router < 4; router++) {
				ParameterId routerId = new ParameterId(p.id.getNumber() + router + 1,
						EditBufferModel.get().voiceGroup.getValue());
				ModulationRouterParameterModel routerParameter = (ModulationRouterParameterModel) EditBufferModel.get()
						.getParameter(routerId);

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
			ParameterId mc = routerParameter.getAssociatedMacroControlID();
			applyModulation(mc, diff * amount);
		}
	}

	private void applyModulation(ParameterId macroControlID, double delta) {
		BasicParameterModel macroControl = EditBufferModel.get().getParameter(macroControlID);

		double oldQ = macroControl.value.getQuantizedAndClipped(true);
		double v = macroControl.value.value.getValue();
		macroControl.value.value.setValue(v + delta);
		double newQ = macroControl.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(macroControlID, newQ - oldQ);
	}

	private void applyModulationToModulateableParameters(ParameterId macroControlID, double d) {
		if (d != 0) {
			ModSource m = ModSource.fromParameterId(macroControlID.getNumber());
			for (ModulateableParameterModel t : EditBufferModel.get().getAllModulateableParameters()) {
				if (t.modSource.getValue() == m) {
					double amount = t.modAmount.getQuantizedAndClipped(true);
					if (t.value.metaData.bipolar.getBool())
						amount *= 2;
					t.value.value.setValue(t.value.value.getValue() + d * amount);
				}
			}

			MacroControlParameterModel mc = EditBufferModel.get().getParameter(m,
					EditBufferModel.get().voiceGroup.getValue());
			handleBidirectionalRibbonBinding(mc);
		}
	}

	private void applyDirectModulation(ModulationRouterParameterModel routerParameter, double newValue) {
		MacroControlParameterModel m = (MacroControlParameterModel) EditBufferModel.get()
				.getParameter(routerParameter.getAssociatedMacroControlID());
		double oldQ = m.value.getQuantizedAndClipped(true);
		m.value.value.setValue(newValue);
		double newQ = m.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(m.id, newQ - oldQ);
	}

	public void selectParameter(int paramNumber) {
		if (EditBufferModel.get().selectedParameter.setValue(paramNumber))
			NonMaps.get().getServerProxy().selectParameter(paramNumber);
	}

	public void incParameter(int paramNumber, boolean fine) {
		incDecParameter(paramNumber, fine, 1);
	}

	public void decParameter(int paramNumber, boolean fine) {
		incDecParameter(paramNumber, fine, -1);
	}

	private void incDecParameter(int paramNumber, boolean fine, int inc) {
		ParameterId id = toParamId(paramNumber);
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
		double v = p.getIncDecValue(fine, inc);
		setParameterValue(id, v, true);
	}

	private ParameterId toParamId(int paramNumber) {
		return new ParameterId(paramNumber, getVoiceGroupFor(paramNumber));
	}

	public void initializeSound() {
		NonMaps.get().getServerProxy().initSound();
	}

	public void randomizeSound() {
		NonMaps.get().getServerProxy().randomizeSound();
	}

	public void convertToSingleSound() {
		NonMaps.get().getServerProxy().convertToSingle();
		EditBufferModel.get().soundType.setValue(SoundType.Single);
	}

	public void convertToSplitSound() {
		NonMaps.get().getServerProxy().convertToSplit();
		EditBufferModel.get().soundType.setValue(SoundType.Split);
	}

	public void convertToLayerSound() {
		NonMaps.get().getServerProxy().convertToLayer();
		EditBufferModel.get().soundType.setValue(SoundType.Layer);
	}

	public void setToDefault(int paramNumber) {
		ParameterId id = toParamId(paramNumber);
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
		double v = p.value.metaData.defaultValue.getValue();
		setParameterValue(id, v, true);
	}

	public void toggleBoolean(int paramNumber) {
		ParameterId id = toParamId(paramNumber);
		BasicParameterModel p = EditBufferModel.get().getParameter(id);

		if (p.value.getQuantizedAndClipped(true) != 0.0)
			setParameterValue(id, 0, true);
		else
			setParameterValue(id, 1, true);
	}

	public void decModulationAmount(int paramNumber, boolean fine) {
		incDecModulationAmount(paramNumber, fine, -1);
	}

	public void incModulationAmount(int paramNumber, boolean fine) {
		incDecModulationAmount(paramNumber, fine, 1);
	}

	public void resetModulationAmount(int paramNumber) {
		setModulationAmount(toParamId(paramNumber), 0, false);
	}

	private void incDecModulationAmount(int paramNumber, boolean fine, int inc) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		double v = p.modAmount.getIncDecValue(fine, inc);
		setModulationAmount(id, v, true);
	}

	public void setModulationAmount(ParameterId id, double newValue, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		double oldValue = p.modAmount.getQuantizedAndClipped(fine);
		p.modAmount.value.setValue(newValue);
		newValue = p.modAmount.getQuantizedAndClipped(fine);
		double diff = newValue - oldValue;

		if (diff != 0)
			NonMaps.get().getServerProxy().setModulationAmount(newValue);
	}

	private void setModulationUpperBound(ParameterId id,  double newAmount, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		ParameterId mcId = new ParameterId(p.modSource.getValue(), id.getVoiceGroup());
		MacroControlParameterModel mc = (MacroControlParameterModel) EditBufferModel.get().getParameter(		mcId);

		double factor = p.value.metaData.bipolar.getBool() ? 2 : 1;
		double oldAmount = p.modAmount.getClippedValue();
		double oldValue = p.value.getClippedValue();
		double mcValue = mc.value.getClippedValue();
		double oldLowerBound = oldValue - (factor * oldAmount) * mcValue;

		setModulationAmount(id, newAmount, true);

		double newLowerBound = oldValue - (factor * newAmount) * mcValue;
		double lowerBoundDiff = newLowerBound - oldLowerBound;
		setParameterValue(id, oldValue - lowerBoundDiff, true);
	}

	private void setModulationLowerBound(ParameterId id, double newAmount, boolean fine) {

		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
				ParameterId mcId = new ParameterId(p.modSource.getValue(), id.getVoiceGroup());
				MacroControlParameterModel mc = (MacroControlParameterModel) EditBufferModel.get().getParameter(		mcId);

		double factor = p.value.metaData.bipolar.getBool() ? 2 : 1;
		double oldAmount = p.modAmount.getClippedValue();
		double oldValue = p.value.getClippedValue();
		double mcValue = mc.value.getClippedValue();
		double oldUpperBound = oldValue + (factor * oldAmount) * (1.0 - mcValue);

		setModulationAmount(id, newAmount, true);

		double newUpperBound = oldValue + (factor * newAmount) * (1.0 - mcValue);
		double upperBoundDiff = newUpperBound - oldUpperBound;
		setParameterValue(id, oldValue - upperBoundDiff, true);
	}

	public void setModulationSource(int paramNumber, ModSource src) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		if (p.modSource.setValue(src))
			NonMaps.get().getServerProxy().setModulationSource(src);
	}

	public IncrementalChanger startEditParameterValue(int paramNumber, double pixelsPerRange) {
		ParameterId id = toParamId(paramNumber);
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.value, pixelsPerRange, (v, b) -> setParameterValue(id, v, true, b), () -> {
			if (p instanceof PhysicalControlParameterModel) {
				PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
				if (m.isReturning())
					startReturningAnimation(m);
			}
		});
	}

	public IncrementalChanger startEditMCAmount(int paramNumber, double pixelsPerRange) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationAmount(id, v, true),
				null);
	}

	public IncrementalChanger startEditMacroControlValue(int paramNumber, double pixelsPerRange) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);

		if (p.modSource.getValue() != ModSource.None)
			return startEditParameterValue(p.modSource.getValue().toParameterId(), pixelsPerRange);

		return null;
	}

	public IncrementalChanger startEditModulationAmountLowerBound(int paramNumber, double pixelsPerRange) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationLowerBound(id, v, true),
				null) {
			@Override
			public double bendAmount(double i) {
				return -i / 2;
			}
		};
	}

	public IncrementalChanger startEditModulationAmountUpperBound(int paramNumber, double pixelsPerRange) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationUpperBound(id, v, true),
				null) {
			@Override
			public double bendAmount(double i) {
				return i / 2;
			}
		};
	}

	public void renameMacroControl(int paramNumber, String newName) {
		ParameterId id = toParamId(paramNumber);
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(id);
		m.givenName.setValue(newName);
		NonMaps.theMaps.getServerProxy().renameMacroControl(id, newName);
	}

	public void setMacroControlInfo(int paramNumber, String text) {
		ParameterId id = toParamId(paramNumber);
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(id);
		m.info.setValue(text);
		NonMaps.theMaps.getServerProxy().setMacroControlInfo(id, text);
	}

	public void selectVoiceGroup(VoiceGroup group) {
		EditBufferModel.get().voiceGroup.setValue(group);
	}

	public void toggleVoiceGroup() {
		if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.I)
			selectVoiceGroup(VoiceGroup.II);
		else if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.II)
			selectVoiceGroup(VoiceGroup.I);
	}

	private VoiceGroup getVoiceGroupFor(int paramNumber) {
		return ParameterFactory.isGlobalParameter(paramNumber) ? VoiceGroup.Global
				: EditBufferModel.get().voiceGroup.getValue();
	}

	public void resetModulation(int id) {
		NonMaps.theMaps.getServerProxy().resetModulation(id, getVoiceGroupFor(id));
	}

}
