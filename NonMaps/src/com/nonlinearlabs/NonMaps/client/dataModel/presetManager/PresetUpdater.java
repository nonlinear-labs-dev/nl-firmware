package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class PresetUpdater extends Updater {

	public void update(Node xml, Preset preset) {
		preset.name.setValue(getAttributeValue(xml, "name"));
		preset.uuid.setValue(getAttributeValue(xml, "uuid"));
	}

}
