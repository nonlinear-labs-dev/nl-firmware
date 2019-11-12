package com.nonlinearlabs.client.dataModel.presetManager;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;

public class PresetUpdater extends Updater {

	protected Preset target;

	public PresetUpdater(Node n, Preset preset) {
		super(n);
		target = preset;
	}

	public void doUpdate() {
		target.name.setValue(getAttributeValue(root, "name"));
		target.uuid.setValue(getAttributeValue(root, "uuid"));
	}

	protected void handlePresetParameters(Node xml, Preset preset) {
	}
}
