package com.nonlinearlabs.client.useCases;

import java.util.Arrays;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.LoadToPartMode.LoadToPartModeData;
import com.nonlinearlabs.client.Millimeter;
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
import com.nonlinearlabs.client.dataModel.editBuffer.SendParameterModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;

public class EditBufferUseCases {
	private static EditBufferUseCases theInstance = new EditBufferUseCases();
	private AnimationManager animationManager = new AnimationManager();

	public static EditBufferUseCases get() {
		return theInstance;
	}

	public void setParameterValue(ParameterId id, double newValue, boolean oracle) {
		setParameterValue(id, newValue, oracle, true);
	}

	public AnimationManager getAnimationManager() {
		return animationManager;
	}

	private void setParameterValue(ParameterId id, double newValue, boolean oracle, boolean setAnimationTimeout) {
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
		double oldValue = p.value.getQuantizedAndClipped(p.value.value.getValue(), true);
		p.value.value.setValue(newValue);

		newValue = p.value.getQuantizedAndClipped(newValue, true);
		double diff = newValue - oldValue;

		if (p instanceof PhysicalControlParameterModel) {
			PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
			applyPhysicalControlModulation(m, diff);

			if (m.isReturning()) {
				if (setAnimationTimeout)
					animationManager.startDelayedAnimation(m, 2000);
				else
					animationManager.cancelAnimation(m);
			}
		}

		if (p instanceof MacroControlParameterModel)
			applyModulationToModulateableParameters(id, newValue);

		if (p instanceof ModulateableParameterModel) {
			updateModulationBase((ModulateableParameterModel) p);
		}

		if (p.id.getNumber() == 356) {
			handleSplitExceptionalParameterChange(p, newValue, oracle);
			return;
		}

		if (!getTestingFlag())
			NonMaps.get().getServerProxy().setParameter(id, newValue, oracle);
	}

	static native boolean getTestingFlag() /*-{
		return window['isCurrentlyTesting'] == 1;
	}-*/;

	public void updateModulationBase(ModulateableParameterModel p) {
		ModulateableParameterModel.ModSource src = p.modSource.getValue();

		if (src != ModulateableParameterModel.ModSource.None) {
			MacroControlParameterModel mc = (MacroControlParameterModel) EditBufferModel.get()
					.getParameter(new ParameterId(p.modSource.getValue()));

			if (p.id.getNumber() == 356)
				updateModulationBaseForSplitpoint(p, mc);
			else
				updateModulationBaseForGenericParam(p, mc);

		} else {
			p.modBase.value.setValue(0.0);
		}
	}

	private void updateModulationBaseForGenericParam(ModulateableParameterModel p, MacroControlParameterModel mc) {
		var range = p.value.metaData.bipolar.getBool() ? 2 : 1;
		var modAmount = p.modAmount.value.getValue() * range;
		var curValue = p.value.value.getValue();
		var m = curValue - modAmount * mc.value.getQuantizedAndClipped(true);
		p.modBase.value.setValue(m);
	}

	// copy of the algorithm used in audio-engine
	private int ae_round(double _value) {
		return (int) Math.ceil(_value * 60);
	}

	private double ae_quantize(double _value, int _steps) {
		if (_steps == 0)
			return _value;

		return Math.max(0, Math.min(1, Math.floor(_value * (1 + _steps)) / _steps));
	}

	double ae_getModulation(double _mod, double modAmount) {
		return modAmount * ae_quantize(_mod, ae_round(Math.abs(modAmount)));
	}

	private void updateModulationBaseForSplitpoint(ModulateableParameterModel p, MacroControlParameterModel mc) {
		var _mod = (float) mc.value.getQuantizedAndClipped(true);
		var modAmount = p.modAmount.value.getValue();
		var curValue = p.value.getQuantizedAndClipped(true);
		var newBase = curValue - (double) ae_getModulation(_mod, modAmount);
		p.modBase.value.setValue(newBase);
	}

	private void handleSplitSync(BasicParameterModel p, double newValue) {
		BasicParameterModel sibling = getSibling(p);
		if (p.id.getVoiceGroup() == VoiceGroup.I) {
			newValue = Math.min(59 / 60.0, newValue);
			p.value.value.setValue(newValue);
			sibling.value.value.setValue(p.value.getQuantizedAndClipped(true) + 1 / 60.0);
		} else {
			newValue = Math.max(1 / 60.0, newValue);
			p.value.value.setValue(newValue);
			sibling.value.value.setValue(p.value.getQuantizedAndClipped(true) - 1 / 60.0);
		}

		NonMaps.get().getServerProxy().setSplitPoints(p.id, p.value.getQuantizedAndClipped(true),
				sibling.value.getQuantizedAndClipped(true), true);
	}

	private BasicParameterModel getSibling(BasicParameterModel p) {
		ParameterId siblingId = new ParameterId(p.id.getNumber(),
				p.id.getVoiceGroup() == VoiceGroup.I ? VoiceGroup.II : VoiceGroup.I);
		return EditBufferModel.get().getParameter(siblingId);
	}

	private void handleSplitExceptionalParameterChange(BasicParameterModel p, double newValue, boolean oracle) {
		if (SetupModel.get().systemSettings.syncSplit.getBool()) {
			handleSplitSync(p, newValue);
		} else {
			if (p.id.getVoiceGroup() == VoiceGroup.I) {
				newValue = Math.min(59 / 60.0, newValue);
				p.value.value.setValue(newValue);
			} else {
				newValue = Math.max(1 / 60.0, newValue);
				p.value.value.setValue(newValue);
			}
			NonMaps.get().getServerProxy().setSplitPoints(p.id, newValue, 0.0, oracle);
		}
	}

	private void startReturningAnimation(PhysicalControlParameterModel m) {
		animationManager.startReturnAnimation(m);
	}

	private void startReturningAnimation(SendParameterModel m) {
		animationManager.startReturnAnimation(m);
	}

	private void handleBidirectionalRibbonBinding(MacroControlParameterModel m) {
		for (ParameterId routerId : m.getRouterIDs()) {
			ModulationRouterParameterModel r = this.<ModulationRouterParameterModel>findParameter(routerId);

			if (r.value.getQuantized(true) > 0.0) {
				ParameterId physicalControlID = r.getAssociatedPhysicalControlID();
				if (Arrays.binarySearch(ParameterFactory.ribbons, physicalControlID.getNumber()) >= 0) {
					RibbonParameterModel ribbon = this.<RibbonParameterModel>findParameter(physicalControlID);
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
		for (ParameterId router : p.getAssociatedModulationRouters()) {
			ModulationRouterParameterModel routerParameter = (ModulationRouterParameterModel) EditBufferModel.get()
					.getParameter(router);

			if (p.isReturning())
				applyReturningModulation(routerParameter, diff);
			else if (routerParameter.value.value.getValue() != 0) {
				applyDirectModulation(routerParameter, p.value.getQuantizedAndClipped(true));
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
		double v = macroControl.value.value.getValue();
		double newValue = v + delta;
		macroControl.value.value.setValue(newValue);
		applyModulationToModulateableParameters(macroControlID,
				macroControl.getValue().value.getQuantizedAndClipped(true));
	}

	private void applyModulationToModulateableParameters(ParameterId macroControlID, double newVal) {
		ModSource m = ModSource.fromParameterId(macroControlID);
		for (ModulateableParameterModel t : EditBufferModel.get().getAllModulateableParameters()) {
			if (t.modSource.getValue() == m) {
				if (t.id.getNumber() == 356) {
					var newValue = t.modBase.value.getValue() + ae_getModulation(newVal, t.modAmount.value.getValue());
					t.value.value.setValue(newValue);
				} else {
					var range = t.value.metaData.bipolar.getBool() ? 2 : 1;
					var newValue = t.modBase.value.getValue() + t.modAmount.value.getValue() * newVal * range;
					t.value.value.setValue(newValue);
				}
			}
		}

		MacroControlParameterModel mc = this.<MacroControlParameterModel>findParameter(macroControlID);
		handleBidirectionalRibbonBinding(mc);
	}

	private void applyDirectModulation(ModulationRouterParameterModel routerParameter, double newValue) {
		MacroControlParameterModel m = (MacroControlParameterModel) EditBufferModel.get()
				.getParameter(routerParameter.getAssociatedMacroControlID());
		m.value.value.setValue(newValue);
		double newQ = m.value.getQuantizedAndClipped(true);
		applyModulationToModulateableParameters(m.id, newQ);
	}

	public void selectParameter(int paramNumber) {
		ParameterId id = toParamId(paramNumber);
		if (EditBufferModel.get().selectedParameter.setValue(id.getNumber()))
			NonMaps.get().getServerProxy().selectParameter(id);
	}


	public void incParameter(int paramNumber, boolean fine) {
		incDecParameter(paramNumber, fine, 1);
	}

	public void incParameter(ParameterId id, boolean fine) {
		incDecParameter(id, fine, 1);
	}

	public void decParameter(int paramNumber, boolean fine) {
		incDecParameter(paramNumber, fine, -1);
	}

	public void decParameter(ParameterId id, boolean fine) {
		incDecParameter(id, fine, -1);
	}

	public void incDecParameter(int paramNumber, boolean fine, int inc) {
		ParameterId id = toParamId(paramNumber);
		incDecParameter(id, fine, inc);
	}

	public void incDecParameter(ParameterId id, boolean fine, int inc) {
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
		ParameterPresenter pp = ParameterPresenterProviders.get().getParameterPresenter(id.getVoiceGroup(),
				id.getNumber());
		if (pp.isFine == false)
			fine = false;
		double v = p.getIncDecValue(fine, inc);
		setParameterValue(id, v, true);
	}

	public ParameterId toParamId(int paramNumber) {
		return new ParameterId(paramNumber, getVoiceGroupFor(paramNumber));
	}

	public void initializeSound() {
		NonMaps.get().getServerProxy().initSound();
	}

	public void randomizeSound() {
		NonMaps.get().getServerProxy().randomizeSound();
	}

	public void convertToSingleSound() {
		NonMaps.get().getServerProxy().convertToSingle(EditBufferModel.get().voiceGroup.getValue());
		EditBufferModel.get().soundType.setValue(SoundType.Single);
	}

	public void convertToSplitSound() {
		NonMaps.get().getServerProxy().convertToSplit(EditBufferModel.get().voiceGroup.getValue());
		EditBufferModel.get().soundType.setValue(SoundType.Split);
	}

	public void convertToLayerSound() {
		NonMaps.get().getServerProxy().convertToLayer(EditBufferModel.get().voiceGroup.getValue());
		EditBufferModel.get().soundType.setValue(SoundType.Layer);
	}

	public void convertToSplitWithFXIOnly() {
		NonMaps.get().getServerProxy().convertSingleToSplitFXIOnly();
		EditBufferModel.get().soundType.setValue(SoundType.Split);
	}

	public void convertToLayerWithFXIOnly() {
		NonMaps.get().getServerProxy().convertSingleToLayerFXIOnly();
		EditBufferModel.get().soundType.setValue(SoundType.Layer);
	}

	public void setToDefault(ParameterId id) {
		NonMaps.get().getServerProxy().defaultParameter(id);
	}

	public void toggleBoolean(int paramNumber) {
		ParameterId id = toParamId(paramNumber);
		BasicParameterModel p = EditBufferModel.get().getParameter(id);

		boolean isOracle = !(p instanceof ModulationRouterParameterModel);

		if (p.value.getQuantizedAndClipped(true) != 0.0)
			setParameterValue(id, 0, isOracle);
		else
			setParameterValue(id, 1, isOracle);
	}

	public void decModulationAmount(ParameterId id, boolean fine) {
		incDecModulationAmount(id, fine, -1);
	}

	public void incModulationAmount(ParameterId id, boolean fine) {
		incDecModulationAmount(id, fine, 1);
	}

	public void resetModulationAmount(ParameterId id) {
		setModulationAmount(id, 0, false);
	}

	private void incDecModulationAmount(ParameterId id, boolean fine, int inc) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		double v = p.modAmount.getIncDecValue(fine, inc);
		setModulationAmount(id, v, true);
	}

	public void setModulationAmount(ParameterId id, double newValue, boolean fine) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		double oldValue = p.modAmount.getQuantizedAndClipped(fine);
		p.modAmount.value.setValue(newValue);
		updateModulationBase(p);
		newValue = p.modAmount.getQuantizedAndClipped(fine);
		double diff = newValue - oldValue;

		if (!getTestingFlag() && diff != 0)
			NonMaps.get().getServerProxy().setModulationAmount(newValue, id);
	}

	private void setModulationLowerBound(ParameterId id, double newAmount, boolean fine) {
		var p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		var mc = EditBufferModel.get().getParameter(p.modSource.getValue());
		var mcVal = mc.value.getQuantizedAndClipped(true);
		var presenter = ParameterPresenterProviders.get().getParameterPresenter(id.getVoiceGroup(), id.getNumber());
		var isBiPolar = presenter.bipolar;
		var cpRange = isBiPolar ? 2 : 1;
		var rangeUpper = presenter.modulation.modRange.upper.clipped;

		var lowerBound = rangeUpper - newAmount * cpRange;
		var minLowerBound = isBiPolar ? -1.0 : 0.0;

		if (lowerBound < minLowerBound) {
			newAmount = (minLowerBound - rangeUpper) / -cpRange;
			lowerBound = minLowerBound;
		}

		var newValue = lowerBound + newAmount * cpRange * mcVal;
		setModulationBounds(id, newAmount, newValue);

	}

	private void setModulationUpperBound(ParameterId id, double newAmount, boolean fine) {
		var p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		var mc = EditBufferModel.get().getParameter(p.modSource.getValue());
		var mcVal = mc.value.getQuantizedAndClipped(true);
		var presenter = ParameterPresenterProviders.get().getParameterPresenter(id.getVoiceGroup(), id.getNumber());
		var isBiPolar = presenter.bipolar;
		var cpRange = isBiPolar ? 2 : 1;
		var rangeLower = presenter.modulation.modRange.lower.clipped;

		var upperBound = rangeLower + newAmount * cpRange;
		var maxUpperBound = 1.0;

		if (upperBound > maxUpperBound)
			newAmount = (maxUpperBound - rangeLower) / cpRange;

		var newValue = rangeLower + newAmount * cpRange * mcVal;
		setModulationBounds(id, newAmount, newValue);
	}

	private void setModulationBounds(ParameterId id, double newAmount, double newParameterValue) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		p.modAmount.value.setValue(newAmount);
		p.value.value.setValue(newParameterValue);
		updateModulationBase(p);

		if (!getTestingFlag())
			NonMaps.get().getServerProxy().setModulationBounds(id, newAmount, newParameterValue);
	}

	public void setModulationSource(ParameterId id, ModSource src) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		if (p.modSource.setValue(src)) {
			updateModulationBase(p);
			NonMaps.get().getServerProxy().setModulationSource(src, id);
		}
	}

	public IncrementalChanger startEditParameterValue(ParameterId id, double pixelsPerRange) {
		BasicParameterModel p = EditBufferModel.get().getParameter(id);
		if (p.value.getValue().metaData.isBoolean.getBool()) {
			pixelsPerRange = Millimeter.toPixels(10);
		}

		return new IncrementalChanger(p.value, pixelsPerRange, (v, b) -> {
			setParameterValue(id, v, true, b);
		}, () -> {
			if (p instanceof PhysicalControlParameterModel) {
				PhysicalControlParameterModel m = (PhysicalControlParameterModel) p;
				if (m.isReturning())
					startReturningAnimation(m);
			}

			if (p instanceof SendParameterModel) {
				SendParameterModel m = (SendParameterModel) p;
				if (m.isReturning())
					startReturningAnimation(m);
			}
		});
	}

	public IncrementalChanger startEditParameterValue(int paramNumber, double pixelsPerRange) {
		ParameterId id = toParamId(paramNumber);
		return startEditParameterValue(id, pixelsPerRange);
	}

	public IncrementalChanger startEditMCAmount(ParameterId id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationAmount(id, v, true), null);
	}

	public IncrementalChanger startEditMacroControlValue(ParameterId id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);

		if (p.modSource.getValue() != ModSource.None)
			return startEditParameterValue(p.modSource.getValue().toParameterId(), pixelsPerRange);

		return null;
	}

	public IncrementalChanger startEditModulationAmountLowerBound(ParameterId id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationLowerBound(id, v, true),
				null) {
			@Override
			public double bendAmount(double i) {
				return -i / 2;
			}
		};
	}

	public IncrementalChanger startEditModulationAmountUpperBound(ParameterId id, double pixelsPerRange) {
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		return new IncrementalChanger(p.modAmount, pixelsPerRange, (v, b) -> setModulationUpperBound(id, v, true),
				null) {
			@Override
			public double bendAmount(double i) {
				return i / 2;
			}
		};
	}

	public void renameMacroControl(ParameterId id, String newName) {
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(id);
		m.givenName.setValue(newName);
		NonMaps.theMaps.getServerProxy().renameMacroControl(id, newName);
	}

	public void setMacroControlInfo(ParameterId id, String text) {
		MacroControlParameterModel m = this.<MacroControlParameterModel>findParameter(id);
		m.info.setValue(text);
		NonMaps.theMaps.getServerProxy().setMacroControlInfo(id, text);
	}

	public void selectVoiceGroup(VoiceGroup group) {
		boolean changed = EditBufferModel.get().voiceGroup.setValue(group);
		if (SetupModel.get().systemSettings.syncVoiceGroups.getBool() && changed) {
			NonMaps.theMaps.getServerProxy().syncVoiceGroup();
		}
	}

	public void toggleVoiceGroup() {
		if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.I)
			selectVoiceGroup(VoiceGroup.II);
		else if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.II)
			selectVoiceGroup(VoiceGroup.I);
	}

	private boolean containsElement(int e, int[] arr) {
		for (int i : arr) {
			if (e == i)
				return true;
		}
		return false;
	}

	private VoiceGroup getVoiceGroupFor(int paramNumber) {
		if (EditBufferModel.get().soundType.getValue() == SoundType.Layer) {
			if (containsElement(paramNumber, ParameterFactory.voicesParameters)) {
				return VoiceGroup.I;
			}
		}

		if (EditBufferModel.get().soundType.getValue() == SoundType.Single) {
			if(containsElement(paramNumber, ParameterFactory.monophonicParameters)) {
				return ParameterFactory.isGlobalParameter(paramNumber) ? VoiceGroup.Global : EditBufferModel.get().voiceGroup.getValue();
			}
			else if(containsElement(paramNumber, ParameterFactory.getGlobalParameters())){
				return VoiceGroup.Global;
			} else {
				return VoiceGroup.I;
			}
		}

		return ParameterFactory.isGlobalParameter(paramNumber) ? VoiceGroup.Global
				: EditBufferModel.get().voiceGroup.getValue();
	}

	public void resetModulation(int id) {
		NonMaps.theMaps.getServerProxy().resetModulation(id, getVoiceGroupFor(id));
	}

	public void toggleDirectLoad(LoadToPartModeData data, VoiceGroup currentDisplayedVoiceGroup) {

		if (SetupModel.get().systemSettings.directLoad.getBool()) {
			SetupModel.get().systemSettings.directLoad.setValue(false);
			NonMaps.theMaps.getServerProxy().setDirectLoadNoLoadToPart("off");
		} else {
			SetupModel.get().systemSettings.directLoad.setValue(true);
			if (data != null && currentDisplayedVoiceGroup != null) {
				NonMaps.theMaps.getServerProxy().setDirectLoadWithLoadToPart("on", data.m_selectedPreset,
						data.m_selectedVoiceGroup, currentDisplayedVoiceGroup);
			} else {
				NonMaps.theMaps.getServerProxy().setDirectLoadNoLoadToPart("on");
			}
		}
	}

	public void toggleSyncSplit() {
		if (SetupModel.get().systemSettings.syncSplit.getBool()) {
			SetupModel.get().systemSettings.syncSplit.setValue(false);
			NonMaps.theMaps.getServerProxy().setSetting("SyncSplit", "off");
		} else {
			SetupModel.get().systemSettings.syncSplit.setValue(true);
			NonMaps.theMaps.getServerProxy().setSetting("SyncSplit", "on");
		}
	}

	public void loadPreset(String uuid) {
		NonMaps.theMaps.getServerProxy().loadPreset(uuid);
	}

	public void loadPresetPart(String uuid, VoiceGroup i) {
		NonMaps.theMaps.getServerProxy().loadPresetPartIntoPart(uuid, i, EditBufferModel.get().voiceGroup.getValue());
	}

	public void loadPresetPartIntoPart(String uuid, VoiceGroup from, VoiceGroup to) {
		NonMaps.get().getServerProxy().loadPresetPartIntoPart(uuid, from, to);
	}

	public void initializePart() {
		NonMaps.get().getServerProxy().initPart(EditBufferModel.get().voiceGroup.getValue());
	}

	public void renamePart(String newName) {
		NonMaps.get().getServerProxy().renamePart(EditBufferModel.get().voiceGroup.getValue(), newName);
	}

	public void renamePart(String newName, VoiceGroup vg) {
		NonMaps.get().getServerProxy().renamePart(vg, newName);
	}

	public void randomize() {
		if (EditBufferModel.get().soundType.getValue() == SoundType.Single)
			NonMaps.get().getServerProxy().randomizeSound();
		else
			NonMaps.get().getServerProxy().randomizePart(EditBufferModel.get().voiceGroup.getValue());
	}

	public void roundParameter(int paramNumber) {
		ParameterId id = toParamId(paramNumber);
		ModulateableParameterModel p = (ModulateableParameterModel) EditBufferModel.get().getParameter(id);
		double rounded = p.value.getQuantizedAndClipped(false);
		setParameterValue(id, rounded, true);
	}

	public void unmutePart(VoiceGroup group) {
		NonMaps.get().getServerProxy().unmutePart(group);
	}

	public void mutePart(VoiceGroup group) {
		NonMaps.get().getServerProxy().mutePart(group);
	}

	public void setMuteForPartAndUnmuteOther(VoiceGroup g) {
		NonMaps.get().getServerProxy().setMuteForPartAndUnmuteOther(g);
	}

	public void selectMidiBank(Bank bank) {
		NonMaps.get().getNonLinearWorld().getPresetManager().setMidiBank(bank);
		NonMaps.get().getServerProxy().selectMidiBank(bank);
	}

}
