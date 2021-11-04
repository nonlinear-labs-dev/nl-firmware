package com.nonlinearlabs.client.dataModel;

import java.util.function.Consumer;

import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;

public abstract class Updater {
	protected Node root;

	public Updater(Node n) {
		root = n;
	}

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
		return getAttributeValue(n, "changed").equals("1");
	}

	protected static String getAttributeValue(Node n, String attributeName) {
		return n.getAttributes().getNamedItem(attributeName).getNodeValue();
	}

	protected void processChangedChildrenElements(Node c, String name, Consumer<Node> cb) {
		processChildrenElements(c, name, t -> {
			if (didChange(t)) {
				cb.accept(t);
			}
		});
	}

	protected void processChildrenElements(Node c, String name, Consumer<Node> cb) {
		if (c != null) {
			c = c.getFirstChild();

			while (c != null) {
				if (c.getNodeType() == Node.ELEMENT_NODE && c.getNodeName().equals(name))
					cb.accept(c);
				c = c.getNextSibling();
			}
		}
	}

	abstract public void doUpdate();
}
