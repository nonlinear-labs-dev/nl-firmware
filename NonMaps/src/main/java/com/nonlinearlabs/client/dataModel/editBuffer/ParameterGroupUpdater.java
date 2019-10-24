package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.Updater;

public class ParameterGroupUpdater extends Updater {
	private ParameterGroupModel target;

	public ParameterGroupUpdater(Node c, ParameterGroupModel target) {
		super(c);
		this.target = target;
	}

	@Override
	public void doUpdate() {
		target.shortName.setValue(getAttributeValue(root, "short-name"));
		target.longName.setValue(getAttributeValue(root, "long-name"));
		processChangedChildrenElements(root, "parameter", c -> processParameter(c));
	}

	private void processParameter(Node c) {
		String id = getAttributeValue(c, "id");
		BasicParameterModel p = EditBufferModel.findParameter(Integer.parseInt(id));
		if (p != null) {
			Updater u = p.getUpdater(c);
			u.doUpdate();
		}
	}
}
