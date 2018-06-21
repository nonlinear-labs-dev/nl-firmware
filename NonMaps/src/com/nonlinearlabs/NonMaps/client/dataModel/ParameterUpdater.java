package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;

public class ParameterUpdater extends Updater {

	private Node root;
	private BasicParameter target;

	public ParameterUpdater(Node c, BasicParameter p) {
		root = c;
		target = p;
	}

	public void doUpdate() {
		ValueUpdater vu = new ValueUpdater(target.value);
		vu.update(root);
	}

}
