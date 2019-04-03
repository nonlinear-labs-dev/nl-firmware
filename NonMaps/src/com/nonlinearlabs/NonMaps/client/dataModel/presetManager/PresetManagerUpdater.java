package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.HashMap;
import java.util.Map;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class PresetManagerUpdater extends Updater {

	protected PresetManager target;
	
	public PresetManagerUpdater(Node c, PresetManager pm) {
		super(c);
		target = pm;
	}

	public void doUpdate() {
		if (didChange(root))
			processChildrenElements(root, "banks", t -> updateBanks(target, t));
	}

	private void updateBanks(PresetManager pm, Node banks) {
		BankMapDataModelEntity existingBanksEntity = pm.getBanks();
		Map<String, Bank> existingBanks = new HashMap<String, Bank>(existingBanksEntity.getValue());
		existingBanks.forEach((uuid, bank) -> bank.setDoomed());
		processChildrenElements(banks, "preset-bank", t -> updateBank(existingBanks, t));
		existingBanks.entrySet().removeIf(e -> e.getValue().isDoomed());
		existingBanksEntity.setValue(existingBanks);
	}

	private void updateBank(Map<String, Bank> existingBanks, Node bank) {
		if (didChange(bank)) {
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
				BankUpdater updater = new BankUpdater(bank, b);
				updater.doUpdate();
			}
		}
	}
}
