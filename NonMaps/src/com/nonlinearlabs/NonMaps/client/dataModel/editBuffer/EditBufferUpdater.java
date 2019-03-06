package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class EditBufferUpdater extends Updater {

	public EditBufferUpdater(Node editBufferNode) {
		super(editBufferNode);
	}

	@Override
	public void doUpdate() {
		if(root != null) {		
			EditBufferModel eb = EditBufferModel.get();
			String selParam = getAttributeValue(root, "selected-parameter");
			eb.selectedParameter.setValue(Integer.valueOf(selParam));
			String loadedPreset = getAttributeValue(root, "loaded-preset");
			eb.loadedPreset.setValue(loadedPreset);
			String loadedPresetName = getAttributeValue(root, "loaded-presets-name");
			eb.loadedPresetName.setValue(loadedPresetName);
			String loadedPresetBankName = getAttributeValue(root, "loaded-presets-bank-name");
			eb.loadedPresetBankName.setValue(loadedPresetBankName);
			String isZombie = getAttributeValue(root, "preset-is-zombie");
			eb.isZombie.setValue(Boolean.valueOf(isZombie));
			String isModified = getAttributeValue(root, "is-modified");
			eb.isModified.setValue(Boolean.valueOf(isModified));
			String changed = getAttributeValue(root, "changed");
			eb.isChanged.setValue(Boolean.valueOf(changed));
		}

		processChangedChildrenElements(root, "parameter-group", child -> processGroup(child));
	}

	private void processGroup(Node c) {
		String groupId = getAttributeValue(c, "id");
		
		ParameterGroupModel target = EditBufferModel.get().getGroup(groupId);
		ParameterGroupUpdater updater = new ParameterGroupUpdater(c, target);
		updater.doUpdate();
	}

}
