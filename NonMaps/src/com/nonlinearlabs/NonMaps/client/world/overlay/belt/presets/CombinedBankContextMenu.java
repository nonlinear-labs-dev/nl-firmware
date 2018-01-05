package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class CombinedBankContextMenu extends BankContextMenu {

	public CombinedBankContextMenu(OverlayLayout parent, final Bank bank) {
		super(parent, bank);
	}

	@Override
	protected boolean hasMinimize() {
		return false;
	}

	@Override
	protected boolean hasPaste() {
		return false;
	}
}
