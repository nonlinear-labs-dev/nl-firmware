package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.HashSet;
import java.util.Set;

import com.google.gwt.xml.client.Document;
import com.google.gwt.xml.client.Node;

public class PresetSearchUpdater extends Updater {

	private Document xml;
	private Set<String> hits = new HashSet<String>();

	public PresetSearchUpdater(Document xml) {
		this.xml = xml;
	}

	public void doUpdate() {
		Node pm = this.xml.getFirstChild();
		processPresetManager(pm);
	}

	private void processPresetManager(Node pm) {
		super.processChildrenElements(pm, "banks", n -> processBanks(n));
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
