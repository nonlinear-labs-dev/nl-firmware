package com.nonlinearlabs.client.dataModel.presetManager;

import java.util.HashSet;
import java.util.Set;

import com.google.gwt.xml.client.Document;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;

public class PresetSearchUpdater extends Updater {

	private Set<String> hits = new HashSet<String>();

	public PresetSearchUpdater(Document xml) {
		super(xml.getFirstChild());
	}

	@Override
	public void doUpdate() {
		processPresetManager();
	}

	private void processPresetManager() {
		super.processChildrenElements(root, "banks", n -> processBanks(n));
		PresetSearch.get().results.setValue(hits);
	}

	private void processBanks(Node banks) {
		super.processChildrenElements(banks, "preset-bank", n -> processBank(n));
	}

	private void processBank(Node bank) {
		super.processChildrenElements(bank, "preset", n -> processPreset(n));
	}

	private void processPreset(Node preset) {
		hits.add(getAttributeValue(preset, "uuid"));
	}

}
