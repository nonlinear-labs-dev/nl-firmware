package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;

public class ParameterGroupModelUpdater extends Updater {
	private ParameterGroupModel target;

	public ParameterGroupModelUpdater(Node c, ParameterGroupModel target, GroupId groupId) {
		super(c);
		this.target = target;
		this.target.id = groupId;
	}

	@Override
	public void doUpdate() {
		target.shortName.setValue(getAttributeValue(root, "short-name"));
		target.longName.setValue(getAttributeValue(root, "long-name"));
		processChangedChildrenElements(root, "parameter", c -> processParameter(c));
	}

	private void processParameter(Node c) {
		String id = getAttributeValue(c, "id");
		BasicParameterModel p = EditBufferModel.get().getParameter(new ParameterId(id));
		if (p != null) {
			p.setGroup(target);
			Updater u = p.createUpdater(c);
			u.doUpdate();
		}
	}
}
