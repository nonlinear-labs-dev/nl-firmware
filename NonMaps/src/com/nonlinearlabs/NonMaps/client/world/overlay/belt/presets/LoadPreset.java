package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

class LoadPreset extends SVGImage {

	LoadPreset(OverlayLayout parent) {
		super(parent, "Load_Enabled.svg", "Load_Active.svg", "Load_Disabled.svg");
	}
	
	public boolean isSelectedBankEmpty() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		String b = pm.getSelectedBank();
		if (b != null) {
			Bank bank = pm.findBank(b);
			if(bank.getPresetList().getPresetCount() != 0)
				return false;
		}
		return true;
	}

	@Override
	public int getSelectedPhase() {
		if(isSelectedBankEmpty()) {
			return drawStates.disabled.ordinal();
		} else if (!isEnabled()) {
			return drawStates.disabled.ordinal();
		} else if (NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode()) {
			return drawStates.disabled.ordinal();
		} else if (isCaptureControl()) {
			return drawStates.active.ordinal();
		} else {
			return drawStates.normal.ordinal();
		}
	};

	@Override
	public Control click(Position eventPoint) {
		if (NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode())
			return this;

		if (isEnabled()) {
			load();
			return this;
		}
		return super.click(eventPoint);
	}

	boolean isEnabled() {
		if (!isSelectedPresetLoaded())
			return true;

		return NonMaps.get().getNonLinearWorld().getParameterEditor().isModified();
	}

	protected boolean isSelectedPresetLoaded() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		String b = pm.getSelectedBank();
		if (b != null) {
			Bank bank = pm.findBank(b);
			if (bank != null) {
				String p = bank.getPresetList().getSelectedPreset();
				if (p != null) {
					Preset preset = bank.getPresetList().findPreset(p);
					if (preset != null) {
						return preset.equals(pm.findLoadedPreset());
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
}
