package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;

public class Updater {
	protected Node getChild(Node node, String... path) {
		return getChild(node, 0, path);
	}

	protected Node getChild(Node node, int offset, String... path) {

		if (offset == path.length)
			return node;

		NodeList nl = node.getChildNodes();
		int count = nl.getLength();

		for (int i = 0; i < count; i++) {
			Node n = nl.item(i);
			String name = n.getNodeName();
			if (name.equals(path[offset])) {
				return getChild(n, offset + 1, path);
			}
		}

		return null;
	}

	protected String getChildText(Node node, String... path) {
		Node n = getChild(node, path);
		return getText(n);
	}

	protected String getText(Node n) {
		if (n != null) {
			NodeList nodes = n.getChildNodes();
			if (nodes != null) {
				Node child = nodes.item(0);
				if (child != null) {
					return child.getNodeValue();
				}
			}
		}
		return "";
	}

	protected static boolean didChange(Node n) {
		return n.getAttributes().getNamedItem("changed").getNodeValue().equals("1");
	}

}
