package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class EditBufferUpdater extends Updater {
	private Node rootNode;

	public EditBufferUpdater(Node editBufferNode) {
		rootNode = editBufferNode;
	}

	public void doUpdate() {
		if(rootNode != null) {		
			EditBufferModel eb = EditBufferModel.get();
			String selParam = getAttributeValue(rootNode, "selected-parameter");
			eb.selectedParameter.setValue(Integer.valueOf(selParam));
			String loadedPreset = getAttributeValue(rootNode, "loaded-preset");
			eb.loadedPreset.setValue(loadedPreset);
			String loadedPresetName = getAttributeValue(rootNode, "loaded-presets-name");
			eb.loadedPresetName.setValue(loadedPresetName);
			String loadedPresetBankName = getAttributeValue(rootNode, "loaded-presets-bank-name");
			eb.loadedPresetBankName.setValue(loadedPresetBankName);
			String isZombie = getAttributeValue(rootNode, "preset-is-zombie");
			eb.isZombie.setValue(Boolean.valueOf(isZombie));
			String isModified = getAttributeValue(rootNode, "is-modified");
			eb.isModified.setValue(Boolean.valueOf(isModified));
			String changed = getAttributeValue(rootNode, "changed");
			eb.isChanged.setValue(Boolean.valueOf(changed));
		}

		processChangedChildrenElements(rootNode, "parameter-group", child -> processGroup(child));
	}

	private void processGroup(Node c) {
		String groupId = getAttributeValue(c, "id");
		
		ParameterGroupModel target = EditBufferModel.get().getGroup(groupId);
		ParameterGroupUpdater updater = new ParameterGroupUpdater(c, target);
		updater.doUpdate();
	}

}
