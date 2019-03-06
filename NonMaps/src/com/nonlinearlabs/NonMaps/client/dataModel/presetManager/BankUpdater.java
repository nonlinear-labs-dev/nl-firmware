package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.ArrayList;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class BankUpdater extends Updater {

	private int pos = 0;
	private Bank bank;

	public BankUpdater(Node c, Bank target) {
		super(c);
		bank = target;
	}
	
	@Override
	public void doUpdate() {
		bank.name.setValue(getAttributeValue(root, "name"));
		bank.uuid.setValue(getAttributeValue(root, "uuid"));

		ArrayList<Preset> existingPresets = new ArrayList<Preset>(bank.getPresets().getValue());
		existingPresets.forEach(p -> p.setDoomed());
		super.processChildrenElements(root, "preset", p -> updatePreset(existingPresets, p));
		existingPresets.removeIf(p -> p.isDoomed());
		bank.getPresets().setValue(existingPresets);
	}

	private void updatePreset(ArrayList<Preset> existingPresets, Node p) {
		if (existingPresets.size() <= pos)
			existingPresets.add(new Preset());

		String uuid = getAttributeValue(p, "uuid");
		Preset preset = existingPresets.get(pos);
		preset.revive();

		if (didChange(p) || !preset.uuid.getValue().equals(uuid)) {
			PresetUpdater updater = new PresetUpdater(p, preset);
			updater.doUpdate();
		}
	}

}
