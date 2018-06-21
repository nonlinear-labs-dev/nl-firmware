package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;

public class ParameterGroupUpdater extends Updater {
	private Node root;
	private ParameterGroup target;

	public ParameterGroupUpdater(Node c, ParameterGroup target) {
		root = c;
		this.target = target;
	}

	public void doUpdate() {
		target.shortName.setValue(getAttributeValue(root, "short-name"));
		target.longName.setValue(getAttributeValue(root, "long-name"));
		processChangedChildrenElements(root, "parameter", c -> processParameter(c));
	}

	private void processParameter(Node c) {
		String id = getAttributeValue(c, "id");
		BasicParameter p = target.findParameter(Integer.parseInt(id));

		if (p != null) {
			ParameterUpdater updater = new ParameterUpdater(c, p);
			updater.doUpdate();
		}
	}

}
