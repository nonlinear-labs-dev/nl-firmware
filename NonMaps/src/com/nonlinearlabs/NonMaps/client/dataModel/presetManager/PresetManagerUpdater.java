package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.Map;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class PresetManagerUpdater extends Updater {

	public PresetManagerUpdater() {
	}

	public void update(Node xml, PresetManager pm) {
		processChildrenElements(xml, "banks", t -> updateBanks(pm, t));
	}

	private void updateBanks(PresetManager pm, Node banks) {
		Map<String, Bank> existingBanks = pm.getBanks();
		existingBanks.forEach((uuid, bank) -> bank.setDoomed());
		processChildrenElements(banks, "preset-bank", t -> updateBank(existingBanks, t));
		existingBanks.entrySet().removeIf(e -> e.getValue().isDoomed());
	}

	private void updateBank(Map<String, Bank> existingBanks, Node bank) {
		String uuid = getAttributeValue(bank, "uuid");
		Bank b = existingBanks.get(uuid);
		boolean dirty = didChange(bank);

		if (b == null) {
			b = new Bank();
			existingBanks.put(uuid, b);
			dirty = true;
		}

		b.revive();

		if (dirty) {
			BankUpdater updater = new BankUpdater();
			updater.update(bank, b);
		}

		// todo: inform clients about removed / added banks
	}
}
