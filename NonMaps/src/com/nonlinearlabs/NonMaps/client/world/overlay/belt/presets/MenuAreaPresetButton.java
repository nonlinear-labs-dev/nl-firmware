package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class MenuAreaPresetButton extends SVGImage {

	public MenuAreaPresetButton(MenuArea parent) {
		super(parent, "Menu_Preset_Enabled_S.svg", "Menu_Preset_Active_S.svg", "Menu_Preset_Disabled_S.svg");
	}

	@Override
	public Control mouseDown(Position pos) {
		Preset p = getPreset();
		if (p != null) {
			Overlay o = getOverlay();
			return o.setContextMenu(pos, new PresetContextMenu(o, p));
		}
		return super.click(pos);
	}
	
	private Overlay getOverlay() {
		return NonMaps.get().getNonLinearWorld().getViewport().getOverlay();
	}

	@Override
	public Control onContextMenu(Position pos) {
		Preset p = getPreset();
		if (p != null) {
			Overlay o = getOverlay();
			return o.setContextMenu(pos, new PresetContextMenu(o, p));
		}
		return super.click(pos);
	}


	Preset getPreset() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			if (bank != null) {
				String presetUUID = bank.getPresetList().getSelectedPreset();
				return bank.getPresetList().findPreset(presetUUID);
			}
		}
		return null;
	}

	private PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}

	boolean hasPreset() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			if (bank != null) {
				String presetUUID = bank.getPresetList().getSelectedPreset();
				return bank.getPresetList().findPreset(presetUUID) != null;
			}
		}
		return false;
	}
	
	@Override
	public int getSelectedPhase() {
		if(hasPreset())
			return drawStates.normal.ordinal();
		else
			return drawStates.disabled.ordinal();
	}
}
