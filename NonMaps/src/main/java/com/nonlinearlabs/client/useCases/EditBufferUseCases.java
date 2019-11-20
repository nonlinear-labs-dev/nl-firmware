package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.Tracer;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.RibbonParameterModel;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();
	private AnimationManager animationManager = new AnimationManager();

	public static EditBufferUseCases get() {
		return theInstance;
	}

	public void setParameterValue(int id, VoiceGroup vg, double newValue, boolean oracle) {
		setParameterValue(id, vg, newValue, oracle, true);
	}

	private void setParameterValue(int id, VoiceGroup vg, double newValue, boolean oracle,
			boolean setAnimationTimeout) {
		BasicParameterModel p = EditBufferModel.get().getParameter(id, vg);
		double oldQ = p.value.getQuantizedAndClipped(true);
		Tracer.log("Set value for " + vg + " " + id + " " + newValue);
		p.value.value.setValue(newValue);
		double newQ = p.value.getQuantizedAndClipped(true);
		double diff = newQ - oldQ;

		if (p instanceof PhysicalControlParameterModel) {
			PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
			applyPhysicalControlModulation(m, diff, vg);

			if (setAnimationTimeout)
				animationManager.startDelayedAnimation(m, vg, 2000);
			else
				animationManager.cancelAnimation(m);
		}

		if (p instanceof MacroControlParameterModel)
			applyModulationToModulateableParameters(id, diff, vg);

		NonMaps.get().getServerProxy().setParameter(id, vg, newValue, oracle);
	}

	private void startReturningAnimation(PhysicalControlParameterModel m, VoiceGroup vg) {
		animationManager.startReturnAnimation(m, vg);
	}

	private void handleBidirectionalRibbonBinding(MacroControlParameterModel m, VoiceGroup vg) {
		for (int routerId : m.getRouterIDs()) {
			ModulationRouterParameterModel r = this.<ModulationRouterParameterModel>findParameter(routerId, vg);

			if (r.value.getQuantized(true) > 0.0) {
				int physicalControlID = r.getAssociatedPhysicalControlID();

				int ribbon1 = 284;
				int ribbon2 = 289;

				if (physicalControlID == ribbon1 || physicalControlID == ribbon2) {
					RibbonParameterModel ribbon = this.<RibbonParameterModel>findParameter(physicalControlID,
							VoiceGroup.Global);
					if (!ribbon.isReturning()) {
						ribbon.value.value.setValue(m.value.getQuantizedAndClipped(true));
					}
				}
			}
		}
	}

	private <T> T findParameter(int id, VoiceGroup vg) {
		return (T) EditBufferModel.get().getParameter(id, vg);
	}

	private void applyPhysicalControlModulation(PhysicalControlParameterModel p, double diff, VoiceGroup vg) {
		if (Math.abs(diff) > 0.0) {
			for (int router = 0; router < 4; router++) {
				int routerId = p.id + router + 1;
				ModulationRouterParameterModel routerParameter = (ModulationRouterParameterModel) EditBufferModel.get()
						.getParameter(routerId, EditBufferModel.get().voiceGroup.getValue());

				if (p.isReturning())
					applyReturningModulation(routerParameter, diff, vg);
				else if (routerParameter.value.value.getValue() != 0) {
					applyDirectModulation(routerParameter, p.value.getQuantizedAndClipped(true), vg);
				}
			}
		}
	}

	private void applyReturningModulation(ModulationRouterParameterModel routerParameter, double diff, VoiceGroup vg) {
		double amount = routerParameter.value.getQuantizedAndClipped(true);

		if (amount != 0.0) {
			int mc = routerParameter.getAssociatedMacroControlID();
			applyModulation(mc, diff * amount, vg);
		}
	}

	private void applyModulation(int macroControlID, double delta, VoiceGroup vg) {
		BasicParameterModel macroControl = EditBufferModel.get().getParameter(macroControlID,
				EditBufferModel.get().voiceGroup.getValue());

		double oldQ = macroControl.value.getQuantizedAndClipped(true);
		double v = macroControl.value.value.getValue();
		macroControl.value.value.setValue(v + delta);
		double newQ = macroControl.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(macroControlID, newQ - oldQ, vg);
	}

	private void applyModulationToModulateableParameters(int macroControlID, double d, VoiceGroup vg) {
		if (d != 0) {
			ModSource m = ModSource.fromParameterId(macroControlID);
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
			handleBidirectionalRibbonBinding(mc, vg);
		}
	}

	private void applyDirectModulation(ModulationRouterParameterModel routerParameter, double newValue, VoiceGroup vg) {
		MacroControlParameterModel m = (MacroControlParameterModel) EditBufferModel.get().getParameter(
				routerParameter.getAssociatedMacroControlID(), EditBufferModel.get().voiceGroup.getValue());
		double oldQ = m.value.getQuantizedAndClipped(true);
		m.value.value.setValue(newValue);
		double newQ = m.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(m.id, newQ - oldQ, vg);
	}

	public void selectParameter(int id) {
		if (EditBufferModel.get().selectedParameter.setValue(id))
			NonMaps.get().getServerProxy().selectParameter(id);
	}

	public void incParameter(int id, boolean fine) {
		incDecParameter(id, fine, 1);
	}

	public void decParameter(int id, boolean fine) {
		incDecParameter(id, fine, -1);
	}

	private void incDecParameter(int id, boolean fine, int inc) {
		VoiceGroup vg = getVoiceGroupFor(id);
		BasicParameterModel p = EditBufferModel.get().getParameter(id, vg);
		double v = p.getIncDecValue(fine, inc);
		setParameterValue(id, vg, v, true);
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

	public void setToDefault(int parameterID) {
		VoiceGroup vg = getVoiceGroupFor(parameterID);
		BasicParameterModel p = EditBufferModel.get().getParameter(parameterID, vg);
		double v = p.value.metaData.defaultValue.getValue();
		setParameterValue(parameterID, vg, v, true);
	}

	public void toggleBoolean(int parameterID) {
		VoiceGroup vg = getVoiceGroupFor(parameterID);
		BasicParameterModel p = EditBufferModel.get().getParameter(parameterID, vg);

		if (p.value.getQuantizedAndClipped(true) != 0.0)
			setParameterValue(parameterID, vg, 0, true);
		else
			setParameterValue(parameterID, vg, 1, true);
	}

	public void decModulationAmount(int id, boolean fine) {
		incDecModulationAmount(id, fine, -1);
	}

	public void incModulationAmount(int id, boolean fine) {
		incDecModulationAmount(id, fine, 1);
	}

	public void resetModulationAmount(int id) {
		setModulationAmount(id, getVoiceGroupFor(id), 0, false);
	}

	private void incDecModulationAmount(int id, boolean fine, int inc) {
		VoiceGroup vg = EditBufferModel.get().voiceGroup.getValue();
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);
		double v = p.modAmount.getIncDecValue(fine, inc);
		setModulationAmount(id, vg, v, true);
	}

	public void setModulationAmount(int id, VoiceGroup vg, double newValue, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);
		double oldValue = p.modAmount.getQuantizedAndClipped(fine);
		p.modAmount.value.setValue(newValue);
		newValue = p.modAmount.getQuantizedAndClipped(fine);
		double diff = newValue - oldValue;

		if (diff != 0)
			NonMaps.get().getServerProxy().setModulationAmount(newValue);
	}

	private void setModulationUpperBound(int parameterId, VoiceGroup vg, double newAmount, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(parameterId,
				EditBufferModel.get().voiceGroup.getValue());
		MacroControlParameterModel mc = EditBufferModel.get().getParameter(p.modSource.getValue(),
				EditBufferModel.get().voiceGroup.getValue());

		double factor = p.value.metaData.bipolar.getBool() ? 2 : 1;
		double oldAmount = p.modAmount.getClippedValue();
		double oldValue = p.value.getClippedValue();
		double mcValue = mc.value.getClippedValue();
		double oldLowerBound = oldValue - (factor * oldAmount) * mcValue;

		setModulationAmount(parameterId, vg, newAmount, true);

		double newLowerBound = oldValue - (factor * newAmount) * mcValue;
		double lowerBoundDiff = newLowerBound - oldLowerBound;
		setParameterValue(parameterId, vg, oldValue - lowerBoundDiff, true);
	}

	private void setModulationLowerBound(int parameterId, VoiceGroup vg, double newAmount, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(parameterId,
				EditBufferModel.get().voiceGroup.getValue());
		MacroControlParameterModel mc = EditBufferModel.get().getParameter(p.modSource.getValue(),
				EditBufferModel.get().voiceGroup.getValue());

		double factor = p.value.metaData.bipolar.getBool() ? 2 : 1;
		double oldAmount = p.modAmount.getClippedValue();
		double oldValue = p.value.getClippedValue();
		double mcValue = mc.value.getClippedValue();
		double oldUpperBound = oldValue + (factor * oldAmount) * (1.0 - mcValue);

		setModulationAmount(parameterId, vg, newAmount, true);

		double newUpperBound = oldValue + (factor * newAmount) * (1.0 - mcValue);
		double upperBoundDiff = newUpperBound - oldUpperBound;
		setParameterValue(parameterId, vg, oldValue - upperBoundDiff, true);
	}

	public void setModulationSource(int id, ModSource src) {
		VoiceGroup vg = getVoiceGroupFor(id);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);
		if (p.modSource.setValue(src))
			NonMaps.get().getServerProxy().setModulationSource(src);
	}

	public IncrementalChanger startEditParameterValue(int id, double pixelsPerRange) {
		VoiceGroup vg = getVoiceGroupFor(id);
		BasicParameterModel p = EditBufferModel.get().getParameter(id, vg);
		return new IncrementalChanger(p.value, pixelsPerRange, (v, b) -> setParameterValue(id, vg, v, true, b), () -> {
			if (p instanceof PhysicalControlParameterModel) {
				PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
				if (m.isReturning())
					startReturningAnimation(m, vg);
			}
		});
	}

	public IncrementalChanger startEditMCAmount(int id, double pixelsPerRange) {
		VoiceGroup vg = getVoiceGroupFor(id);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationAmount(id, vg, v, true),
				null);
	}

	public IncrementalChanger startEditMacroControlValue(int id, double pixelsPerRange) {
		VoiceGroup vg = getVoiceGroupFor(id);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);

		if (p.modSource.getValue() != ModSource.None)
			return startEditParameterValue(p.modSource.getValue().toParameterId(), pixelsPerRange);

		return null;
	}

	public IncrementalChanger startEditModulationAmountLowerBound(int id, double pixelsPerRange) {
		VoiceGroup vg = getVoiceGroupFor(id);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationLowerBound(id, vg, v, true),
				null) {
			@Override
			public double bendAmount(double i) {
				return -i / 2;
			}
		};
	}

	public IncrementalChanger startEditModulationAmountUpperBound(int id, double pixelsPerRange) {
		VoiceGroup vg = getVoiceGroupFor(id);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id, vg);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationUpperBound(id, vg, v, true),
				null) {
			@Override
			public double bendAmount(double i) {
				return i / 2;
			}
		};
	}

	public void renameMacroControl(int parameterID, String newName) {
		VoiceGroup vg = getVoiceGroupFor(parameterID);
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(parameterID, vg);
		m.givenName.setValue(newName);
		NonMaps.theMaps.getServerProxy().renameMacroControl(parameterID, vg, newName);
	}

	public void setMacroControlInfo(int id, String text) {
		VoiceGroup vg = getVoiceGroupFor(id);
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(id, vg);
		m.info.setValue(text);
		NonMaps.theMaps.getServerProxy().setMacroControlInfo(id, vg, text);
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

	private VoiceGroup getVoiceGroupFor(int parameterId) {
		return ParameterFactory.isGlobalParameter(parameterId) ? VoiceGroup.Global
				: EditBufferModel.get().voiceGroup.getValue();
	}

	public void resetModulation(int id) {
		NonMaps.theMaps.getServerProxy().resetModulation(id, getVoiceGroupFor(id));
	}

}
