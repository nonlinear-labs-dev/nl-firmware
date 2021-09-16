package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

class LoadPreset extends SVGImage {

	LoadPreset(OverlayLayout parent) {
		super(parent, "Load_Enabled.svg", "Load_Active.svg", "Load_Disabled.svg");
	}

	public boolean isSelectedBankEmpty() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		String b = pm.getSelectedBank();
		Bank bank = pm.findBank(b);
		if (bank != null) {
			if (bank.getPresetList().getPresetCount() != 0)
				return false;
		}
		return true;
	}

	@Override
	public int getSelectedPhase() {
		if (isSelectedBankEmpty()) {
			return drawStates.disabled.ordinal();
		} else if (!isEnabled()) {
			return drawStates.disabled.ordinal();
		} else if (NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode()) {
			return drawStates.disabled.ordinal();
		} else if (isCaptureControl()) {
			return drawStates.active.ordinal();
		} else if (NonMaps.theMaps.getNonLinearWorld().getPresetManager().isChangingPresetWhileInDirectLoad()) {
			return drawStates.disabled.ordinal();
		} else {
			return drawStates.normal.ordinal();
		}
	};

	@Override
	public Control click(Position eventPoint) {
		if (NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode())
			return this;

		if(isEnabled() && NonMaps.get().getNonLinearWorld().getPresetManager().isInLoadToPartMode()) {
			loadPart();
			return this;
		}

		if (isEnabled()) {
			load();
			return this;
		}
		return super.click(eventPoint);
	}

	boolean isEnabled() {
		boolean isDL = SetupModel.get().systemSettings.directLoad.getBool();
		
		if (!isSelectedPresetLoaded() && !isDL)
			return true;

		return EditBufferPresenterProvider.getPresenter().isAnyParameterChanged;
	}

	protected boolean isSelectedPresetLoaded() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		boolean isLoadToPartActive = pm.isInLoadToPartMode();

		if(isLoadToPartActive) {
			return false;
		}

		String loadedPresetUUID = EditBufferModel.get().loadedPreset.getValue();

		String b = pm.getSelectedBank();
		if (b != null) {
			Bank bank = pm.findBank(b);
			if (bank != null) {
				String p = bank.getPresetList().getSelectedPreset();
				if (p != null) {
					Preset preset = bank.getPresetList().findPreset(p);
					if (preset != null) {
						return preset.getUUID().equals(loadedPresetUUID);
					}
				}
			}
		}
		return false;
	}

	public void load() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.loadSelectedPreset();
	}

	public void loadPart() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.loadSelectedPresetPart();
	}
}
