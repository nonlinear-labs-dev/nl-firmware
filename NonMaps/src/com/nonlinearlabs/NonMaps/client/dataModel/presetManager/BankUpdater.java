package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.ArrayList;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class BankUpdater extends Updater {

	private int pos = 0;

	public void update(Node xml, Bank dataModelBank) {
		dataModelBank.name.setValue(getAttributeValue(xml, "name"));
		dataModelBank.uuid.setValue(getAttributeValue(xml, "uuid"));

		ArrayList<Preset> existingPresets = dataModelBank.getPresets();
		dataModelBank.getPresets().forEach(p -> p.setDoomed());
		super.processChildrenElements(xml, "preset", p -> updatePreset(existingPresets, p));
		existingPresets.removeIf(p -> p.isDoomed());
	}

	private void updatePreset(ArrayList<Preset> existingPresets, Node p) {
		if (existingPresets.size() <= pos)
			existingPresets.add(new Preset());

		String uuid = getAttributeValue(p, "uuid");
		Preset preset = existingPresets.get(pos);
		preset.revive();

		if (didChange(p) || !preset.uuid.equals(uuid)) {
			PresetUpdater updater = new PresetUpdater();
			updater.update(p, preset);
		}

		// todo: inform clients about removed / added presets
	}

}
