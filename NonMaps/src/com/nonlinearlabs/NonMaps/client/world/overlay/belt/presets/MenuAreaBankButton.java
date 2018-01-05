package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public class MenuAreaBankButton extends MenuAreaButton {

	public MenuAreaBankButton(MenuArea parent) {
		super(parent, "Menu_Bank_Disabled_S.svg", "Menu_Bank_Enabled_S.svg", "Menu_Bank_Active_S.svg", "Menu_Bank_Disabled_L.svg",
				"Menu_Bank_Enabled_L.svg", "Menu_Bank_Active_L.svg");
	}

	@Override
	public Control mouseDown(Position pos) {
		Overlay o = getOverlay();
		PresetManager pm = getPresetManager();

		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			return o.setContextMenu(pos, new CombinedBankContextMenu(o, bank));
		}
		return o.setContextMenu(pos, new CombinedBankContextMenu(o, null));
	}

	@Override
	protected State getState() {
		return State.Enabled;
	}

}
