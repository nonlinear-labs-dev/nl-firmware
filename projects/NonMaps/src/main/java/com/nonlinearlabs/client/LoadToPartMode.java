package com.nonlinearlabs.client;

import java.util.function.Function;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.world.NonLinearWorld;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;

public class LoadToPartMode {

	private class LoadToPartModeData {
		public Preset selectedPreset;
		public Bank selectedBank;
		public VoiceGroup selectedVoiceGroup;
	};

	private PresetManager m_parent;

	private final Preset originPreset;
	
	private LoadToPartModeData currentData;
	private LoadToPartModeData otherData;

	private LoadToPartModeNotifier m_notifier;

	public LoadToPartMode(PresetManager parent) {
		m_parent = parent;

		m_notifier = new LoadToPartModeNotifier();

		currentData = new LoadToPartModeData();
		otherData = new LoadToPartModeData();

		originPreset = parent.findLoadedPreset();

		currentData.selectedPreset = m_parent.getSelectedPreset();
        currentData.selectedBank = m_parent.findBank(m_parent.getSelectedBank());
		
		otherData.selectedBank = currentData.selectedBank;
		otherData.selectedPreset = currentData.selectedPreset;
		otherData.selectedVoiceGroup = currentData.selectedVoiceGroup;

		EditBufferModel.get().voiceGroup.onChange((vg) -> {
			swapSelection();
			return true;
		});
    }

	private void swapSelection() {
		LoadToPartModeData tmp = currentData;
		currentData = otherData;
		otherData = tmp;
		updateUI();
	}

	public void setSelectedPreset(Preset p) {
		currentData.selectedPreset = p;
		currentData.selectedBank = p != null ? p.getParent() : null;
		updateUI();

		if(SetupModel.get().systemSettings.directLoad.getBool()) {
			NonMaps.get().getNonLinearWorld().getPresetManager().loadSelectedPresetPart();
		}
	}

	public void setSelectedBank(Bank b) {
		currentData.selectedBank = b;
		setSelectedPreset(b.getPresetList().findPreset(b.getPresetList().getSelectedPreset()));
		updateUI();
	}

	public Preset getSelectedPreset() {
		return currentData.selectedPreset;
	}

	public VoiceGroup getSelectedPart() {
		return currentData.selectedVoiceGroup;
	}

	public boolean canNext() {
		if (currentData.selectedPreset != null) {
			boolean isLastPreset = currentData.selectedPreset.getNumber() < currentData.selectedPreset.getParent().getPresetList().getPresetCount();

			if(currentData.selectedPreset.isDual()) {
				if(currentData.selectedVoiceGroup == VoiceGroup.I)
					return true;
				else
					return isLastPreset;
			} else {
				return isLastPreset;
			}
		}

		return false;
	}

	public boolean canPrev() {

		if (currentData.selectedPreset != null) {
			boolean isFirstPreset = currentData.selectedPreset.getNumber() > 1;

			if(currentData.selectedPreset.isDual()) {
				if(currentData.selectedVoiceGroup == VoiceGroup.II)
					return true;
				return isFirstPreset;
			} else {
				return isFirstPreset;
			}
		}

		return false;
	}

	public void selectNextPreset() {
		if (canNext()) {
			boolean selectionChanged = false;

			if(currentData.selectedPreset.isDual() && currentData.selectedVoiceGroup == VoiceGroup.I) {
				currentData.selectedVoiceGroup = VoiceGroup.II;
				selectionChanged = true;
			}

			Preset next = currentData.selectedPreset;

			if(!selectionChanged) {
				Bank b = currentData.selectedPreset.getParent();
				int idx = currentData.selectedPreset.getNumber() - 1;
				next = b.getPreset(idx + 1);	
				currentData.selectedVoiceGroup = VoiceGroup.I;
			}
			
			setSelectedPreset(next);
		}
	}

	public void selectPreviousPreset() {
		if (canPrev()) {
			boolean selectionChanged = false;

			if(currentData.selectedPreset.isDual() && currentData.selectedVoiceGroup == VoiceGroup.II) {
				currentData.selectedVoiceGroup = VoiceGroup.I;
				selectionChanged = true;
			}

			Preset next = currentData.selectedPreset;

			if(!selectionChanged) {
				Bank b = currentData.selectedPreset.getParent();
				int idx = currentData.selectedPreset.getNumber() - 1;
				next = b.getPreset(idx - 1);
			
				if(next.isDual()) {
					currentData.selectedVoiceGroup = VoiceGroup.II;
				} else {
					currentData.selectedVoiceGroup = VoiceGroup.I;
				}
			}
			
			setSelectedPreset(next);
		}
	}

	public boolean canSelectPreviousBank() {
		if (currentData.selectedBank != null) {
			return currentData.selectedBank.getParent().canSelectBankWithOrdernumberOffset(currentData.selectedBank, -1);
		}
		return false;
	}

	public boolean canSelectNextBank() {
		if (currentData.selectedBank != null) {
			return currentData.selectedBank.getParent().canSelectBankWithOrdernumberOffset(currentData.selectedBank, 1);
		}
		return false;
	}

	public void selectNextBank() {
		if (canSelectNextBank()) {
			int orderNumber = currentData.selectedBank.getOrderNumber();
			selectBankWithOrderNumber(orderNumber + 1);
		}
	}

	public void selectPreviousBank() {
		if (canSelectPreviousBank()) {
			int orderNumber = currentData.selectedBank.getOrderNumber();
			selectBankWithOrderNumber(orderNumber - 1);
		}
	}

	private void selectBankWithOrderNumber(int i) {
		for (MapsControl c : currentData.selectedBank.getParent().getChildren()) {
			if (c instanceof Bank) {
				Bank b = (Bank) c;
				if (b.getOrderNumber() == i) {
					setSelectedBank(b);
					return;
				}
			}
		}
	}

	public Preset getOriginalPreset() {
		return originPreset;
	}

	public Bank getSelectedBank() {
		return currentData.selectedBank;
	}

	public void onChange(Function<Void, Boolean> cb) {
		m_notifier.onChange(cb);
	}

	public void updateUI() {
		NonMaps.get().getNonLinearWorld().invalidate(NonLinearWorld.INVALIDATION_FLAG_UI_CHANGED);
		NonMaps.get().getNonLinearWorld().getViewport().getOverlay().getBelt().getPresetLayout().getBankControl()
				.update();
				
		m_notifier.notifyChanges();
	}

	public boolean isOriginalPreset(Preset mapsPreset) {
		return mapsPreset == originPreset;
	}
}
