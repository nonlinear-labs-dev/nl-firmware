package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;

public class ParameterUpdater extends Updater {

	private Node root;
	private BasicParameterModel target;

	public ParameterUpdater(Node c, BasicParameterModel p) {
		root = c;
		target = p;
	}

	public void doUpdate() {
		String shortName = getChildText(root, "short-name");
		String longName = getChildText(root, "short-name");

		if (!shortName.isEmpty())
			target.shortName.setValue(shortName);

		if (!longName.isEmpty())
			target.longName.setValue(longName);

		ValueUpdater vu = new ValueUpdater(target.value);
		vu.update(root);
	}

}
