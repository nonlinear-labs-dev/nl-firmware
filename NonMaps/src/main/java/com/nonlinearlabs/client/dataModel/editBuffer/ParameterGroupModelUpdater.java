package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class ParameterGroupModelUpdater extends Updater {
	private ParameterGroupModel target;
	private VoiceGroup vg;

	public ParameterGroupModelUpdater(Node c, ParameterGroupModel target, String groupId, VoiceGroup vg) {
		super(c);
		this.target = target;
		this.target.id = groupId;
		this.vg = vg;
	}

	@Override
	public void doUpdate() {
		target.shortName.setValue(getAttributeValue(root, "short-name"));
		target.longName.setValue(getAttributeValue(root, "long-name"));
		processChangedChildrenElements(root, "parameter", c -> processParameter(c));
	}

	private void processParameter(Node c) {
		String id = getAttributeValue(c, "id");
		BasicParameterModel p = EditBufferModel.get().getOrCreateParameter(Integer.parseInt(id), this.vg);
		if (p != null) {
			p.setGroup(target);
			Updater u = p.createUpdater(c);
			u.doUpdate();
		}
	}
}
