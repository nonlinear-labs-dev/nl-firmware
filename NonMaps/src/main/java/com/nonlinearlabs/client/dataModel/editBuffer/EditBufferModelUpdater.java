package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class EditBufferModelUpdater extends Updater {

	public EditBufferModelUpdater(Node editBufferNode) {
		super(editBufferNode);
	}

	@Override
	public void doUpdate() {
		if (root != null) {
			String selParam = getAttributeValue(root, "selected-parameter");
			EditBufferModel.get().selectedParameter.setValue(Integer.valueOf(selParam));
			String loadedPreset = getAttributeValue(root, "loaded-preset");
			EditBufferModel.get().loadedPreset.setValue(loadedPreset);
			String loadedPresetName = getAttributeValue(root, "loaded-presets-name");
			EditBufferModel.get().loadedPresetName.setValue(loadedPresetName);
			String loadedPresetBankName = getAttributeValue(root, "loaded-presets-bank-name");
			EditBufferModel.get().loadedPresetBankName.setValue(loadedPresetBankName);
			String isZombie = getAttributeValue(root, "preset-is-zombie");
			EditBufferModel.get().isZombie.setValue(Boolean.valueOf(isZombie));
			String isModified = getAttributeValue(root, "is-modified");
			EditBufferModel.get().isModified.setValue(Boolean.valueOf(isModified));
			String changed = getAttributeValue(root, "changed");
			EditBufferModel.get().isChanged.setValue(Boolean.valueOf(changed));

			processChildrenElements(root, "editbuffer-type", n -> {
				String soundType = getText(n);
				EditBufferModel.get().soundType.setValue(EditBufferModel.SoundType.valueOf(soundType));
			});

			processChangedChildrenElements(root, "recall-data", c -> processOriginal(c));
		}

		processChangedChildrenElements(root, "global-parameters", child -> {
			processChangedChildrenElements(root, "parameter-group", group -> processGroup(group, VoiceGroup.Global));
		});

		processChangedChildrenElements(root, "voice-group-I-parameters", child -> {
			processChangedChildrenElements(root, "parameter-group", group -> processGroup(group, VoiceGroup.I));
		});

		processChangedChildrenElements(root, "voice-group-I-parameters", child -> {
			processChangedChildrenElements(root, "parameter-group", group -> processGroup(group, VoiceGroup.II));
		});
	}

	private void processOriginal(Node c) {
		processChildrenElements(c, "global-parameters", child -> {
			processChildrenElements(c, "param", p -> processOriginalParameter(p, VoiceGroup.Global));
		});

		processChildrenElements(c, "voice-group-I-parameters", child -> {
			processChildrenElements(c, "param", p -> processOriginalParameter(p, VoiceGroup.I));
		});

		processChildrenElements(c, "voice-group-II-parameters", child -> {
			processChildrenElements(c, "param", p -> processOriginalParameter(p, VoiceGroup.II));
		});
	}

	private void processGroup(Node c, VoiceGroup vg) {
		String groupId = getAttributeValue(c, "id");

		ParameterGroupModel target = EditBufferModel.get().getOrCreateGroup(groupId, vg);
		ParameterGroupModelUpdater updater = new ParameterGroupModelUpdater(c, target, groupId, vg);
		updater.doUpdate();
	}

	private void processOriginalParameter(Node param, VoiceGroup vg) {
		if (param != null) {
			String id = getAttributeValue(param, "id");
			String val = getAttributeValue(param, "value");
			String modSrc = getAttributeValue(param, "mod-src");
			String modAmt = getAttributeValue(param, "mod-amt");

			if (!id.isEmpty()) {
				BasicParameterModel bpm = EditBufferModel.get().getOrCreateParameter(Integer.valueOf(id), vg);
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
