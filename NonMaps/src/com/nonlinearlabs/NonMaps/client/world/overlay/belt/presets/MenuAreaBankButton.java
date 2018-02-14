package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage.drawStates;

public class MenuAreaBankButton extends SVGImage {

	public MenuAreaBankButton(MenuArea parent) {
		super(parent, "Menu_Bank_Enabled_S.svg", "Menu_Bank_Active_S.svg", "Menu_Bank_Disabled_S.svg", "Menu_A_Enabled.svg",
				"Menu_A_Active.svg", "Menu_A_Disabled.svg");
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

	private PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}

	private Overlay getOverlay() {
		return NonMaps.get().getNonLinearWorld().getViewport().getOverlay();
	}

	@Override
	public Control onContextMenu(Position pos) {
		Overlay o = getOverlay();
		PresetManager pm = getPresetManager();

		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			return o.setContextMenu(pos, new CombinedBankContextMenu(o, bank));
		}
		return o.setContextMenu(pos, new CombinedBankContextMenu(o, null));
	}

	boolean hasBank() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			return pm.findBank(bankUUID) != null;
		}
		return false;
	}

	@Override
	public int getSelectedPhase() {
		if (((MenuArea) getParent()).isSmall() == false)
			return drawStates.normal.ordinal() + 3;
		return drawStates.normal.ordinal();
	}
}
