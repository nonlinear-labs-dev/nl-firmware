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
			String selParam = getAttributeValue(rootNode, "selected-parameter");
			EditBufferModel.get().selectedParameter.setValue(Integer.valueOf(selParam));
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
