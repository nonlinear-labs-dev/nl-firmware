package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public class MenuAreaPresetButton extends MenuAreaButton {

	public MenuAreaPresetButton(MenuArea parent) {
		super(parent, "Menu_Preset_Disabled_S.svg", "Menu_Preset_Enabled_S.svg", "Menu_Preset_Active_S.svg", "Menu_Preset_Disabled_L.svg",
				"Menu_Preset_Enabled_L.svg", "Menu_Preset_Active_L.svg");
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

	Preset getPreset() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			if (bank != null) {
				String presetUUID = bank.getSelectedPreset();
				return bank.findPreset(presetUUID);
			}
		}
		return null;
	}

	@Override
	protected State getState() {
		return getPreset() != null ? State.Enabled : State.Disabled;
	}
}
