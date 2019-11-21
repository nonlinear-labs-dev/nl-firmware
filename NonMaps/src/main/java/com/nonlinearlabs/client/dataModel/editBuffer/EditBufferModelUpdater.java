package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class EditBufferModelUpdater extends Updater {

	public EditBufferModelUpdater(Node editBufferNode) {
		super(editBufferNode);
	}

	@Override
	public void doUpdate() {
		if (root != null) {
			String selParam = getAttributeValue(root, "selected-parameter");
			EditBufferModel.selectedParameter.setValue(Integer.valueOf(selParam));
			String loadedPreset = getAttributeValue(root, "loaded-preset");
			EditBufferModel.loadedPreset.setValue(loadedPreset);
			String loadedPresetName = getAttributeValue(root, "loaded-presets-name");
			EditBufferModel.loadedPresetName.setValue(loadedPresetName);
			String loadedPresetBankName = getAttributeValue(root, "loaded-presets-bank-name");
			EditBufferModel.loadedPresetBankName.setValue(loadedPresetBankName);
			String isZombie = getAttributeValue(root, "preset-is-zombie");
			EditBufferModel.isZombie.setValue(Boolean.valueOf(isZombie));
			String isModified = getAttributeValue(root, "is-modified");
			EditBufferModel.isModified.setValue(Boolean.valueOf(isModified));
			String changed = getAttributeValue(root, "changed");
			EditBufferModel.isChanged.setValue(Boolean.valueOf(changed));

			processChildrenElements(root, "sound-type", n -> {
				String soundType = getText(n);
				EditBufferModel.soundType.setValue(EditBufferModel.SoundType.valueOf(soundType));
			});

			processChangedChildrenElements(root, "recall-data", c -> processOriginal(c));
		}

		processChangedChildrenElements(root, "parameter-group", child -> processGroup(child));
	}

	private void processOriginal(Node c) {
		processChildrenElements(c, "param", child -> processOriginalParameter(child));
	}

	private void processGroup(Node c) {
		String groupId = getAttributeValue(c, "id");

		ParameterGroupModel target = EditBufferModel.getGroup(groupId);
		ParameterGroupModelUpdater updater = new ParameterGroupModelUpdater(c, target, groupId);
		updater.doUpdate();
	}

	private void processOriginalParameter(Node param) {
		if (param != null) {
			String id = getAttributeValue(param, "id");
			String val = getAttributeValue(param, "value");
			String modSrc = getAttributeValue(param, "mod-src");
			String modAmt = getAttributeValue(param, "mod-amt");

			if (!id.isEmpty()) {
				BasicParameterModel bpm = EditBufferModel.findParameter(Integer.valueOf(id));
				if (!val.isEmpty())
					bpm.originalValue.setValue(Double.valueOf(val));

				if (bpm instanceof ModulateableParameterModel) {
					ModulateableParameterModel modP = ((ModulateableParameterModel) bpm);
					if (!modAmt.isEmpty())
						modP.ogModAmount.setValue(Double.valueOf(modAmt));
					if (!modSrc.isEmpty())
						modP.ogModSource.setValue(ModSource.values()[Integer.valueOf(modSrc)]);
				}
			}
		}
	}
}
